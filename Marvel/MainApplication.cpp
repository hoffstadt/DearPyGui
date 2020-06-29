#include "include/MainApplication.h"
#include "mvPythonModule.h"
#include "Core/PythonInterfaces/mvStdOutput.h"
#include "include/mvModuleInitializer.h"
#include "Core/mvWindow.h"
#include "Core/mvAppEditor.h"
#include "Platform/Windows/mvWindowsWindow.h"
#include <iostream>
#include <fstream>
#include "mvInterfaceRegistry.h"
#include "Core/PythonInterfaces/mvInterfaces.h"
#include <CLI11.hpp>
#include <iostream>
#include "Core/StandardWindows/mvSourceWindow.h"

namespace Marvel {

	mvAppLog* mvAppLog::s_instance = nullptr;

	Application::Application(const char* name, int argc, char* argv[]) : argc(argc), argv(argv)
	{
		mvAppEditor::GetAppEditor()->setProgramName(name);
	}

	Application::~Application()
	{
		Py_XDECREF(m);

		PyMem_RawFree(program);

		// shutdown the interpreter
		if (Py_FinalizeEx() < 0)
			exit(120);
	}

	void Application::showConsole()
	{
		HWND hWnd = GetConsoleWindow();
		ShowWindow(hWnd, SW_SHOW);
	}

	void Application::hideConsole()
	{
		HWND hWnd = GetConsoleWindow();
		ShowWindow(hWnd, SW_HIDE);
	}

	int Application::parseCommandLine()
	{
		CLI::App app{ "Marvel Sandbox" };
		app.allow_windows_style_options();

		// options
		app.add_option("-a, --app", AppName, "Name of the python file (without extension)");
		app.add_option("-p, --path", PathName, "Path to app file (default is location of MarvelSandbox.exe)");

		// flags
		app.add_flag("-l, --logger", logger, "Show Logger");
		app.add_flag("-m, --metrics", metrics, "Show Metrics");
		app.add_flag("-s, --source", source, "Show Source");
		app.add_flag("-d, --documentation", documentation, "Sets MarvelSandbox to Documentation Mode");
		app.add_flag("-e, --editor", editorMode, "Sets MarvelSandbox to Editor Mode");

		CLI11_PARSE(app, argc, argv);

		return 0;

	}

	void Application::handlePaths()
	{
		PathName = PathName + ";python38.zip";

		program = Py_DecodeLocale(argv[0], NULL);
		if (program == NULL) {
			fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
			exit(1);
		}
		Py_SetProgramName(program);  /* optional but recommended */

		if (argc < 2) // ran from visual studio
		{
			ranFromVS = true;
			addedPath = std::string(MV_MAIN_DIR) + std::string("MarvelSandbox/");
			PathName = "python38.zip;../../MarvelSandbox";
		}
	}

	int Application::initializePython()
	{
		// add our custom module
		PyImport_AppendInittab("sandboxout", &PyInit_embOut);

		// set path and start the interpreter
		wchar_t* deco = Py_DecodeLocale(PathName.c_str(), nullptr);
		Py_SetPath(deco);
		Py_NoSiteFlag = 1; // this must be set to 1

		Py_Initialize();
		if (!Py_IsInitialized())
		{
			printf("Error initializing Python interpreter\n");
			return 1;
		}
		PyEval_InitThreads();

		// import our custom module to capture stdout/stderr
		m = PyImport_ImportModule("sandboxout");
		PySys_SetObject("stdout", m);
		PySys_SetObject("stderr", m);

		return 0;
	}

	int Application::run()
	{
		if (editorMode)
			return runEditorMode();

		if (documentation)
			return runDocMode();

		if (errorMode)
			return runErrorMode();

		return runRegularMode();

	}

	int Application::runEditorMode()
	{
		mvWindow* window = new mvWindowsWindow(mvAppEditor::GetAppEditor()->getWindowWidth(),
			mvAppEditor::GetAppEditor()->getWindowHeight(), true);
		window->show();
		window->run();

		return 0;
	}

	int Application::runDocMode()
	{
		mvWindow* window = new mvWindowsWindow(mvAppEditor::GetAppEditor()->getWindowWidth(),
			mvAppEditor::GetAppEditor()->getWindowHeight(), false, false, true);
		window->show();
		window->run();

		return 0;
	}

	int Application::runErrorMode()
	{
		PyErr_Print();
		mvApp::GetApp()->showLogger();

		// create window
		mvWindow* window = new mvWindowsWindow(mvApp::GetApp()->getWindowWidth(), mvApp::GetApp()->getWindowHeight(),
			false, true);
		window->show();
		window->run();
		delete window;

		return 0;
	}

	int Application::runRegularMode()
	{
		// returns the dictionary object representing the module namespace
		PyObject* pDict = PyModule_GetDict(pModule); // borrowed reference
		mvApp::GetApp()->setModuleDict(pDict);
		std::string filename = addedPath + std::string(AppName) + ".py";
		mvSourceWindow::GetWindow()->setFile(filename);
		PyEval_SaveThread(); // releases global lock
		mvApp::GetApp()->preRender();
		mvApp::GetApp()->setStarted();
		//mvApp::GetApp()->popParent();

		if (logger) mvApp::GetApp()->showLogger();
		if (source) mvApp::GetApp()->showSource();
		if (metrics) mvApp::GetApp()->showMetrics();
		if (documentation) mvApp::GetApp()->showDoc();

		// create window
		mvWindow* window = new mvWindowsWindow(mvApp::GetApp()->getWindowWidth(), mvApp::GetApp()->getWindowHeight());
		window->show();
		window->run();
		PyGILState_STATE gstate = PyGILState_Ensure();
		Py_XDECREF(pModule);
		delete window;

		return 0;
	}

	void Application::logInformation()
	{
		// info
		mvAppLog::getLogger()->AddLog("[Sandbox Version] %0s\n", mvApp::GetVersion());
		mvAppLog::getLogger()->AddLog("[Python Version] %0s\n", PY_VERSION);
		mvAppLog::getLogger()->AddLog("[ImGui Version] %0s\n", IMGUI_VERSION);
		mvAppLog::getLogger()->AddLog("[Compiler] MSVC version %0d\n", _MSC_VER);
	}

	void Application::importModule()
	{
		// get module
		pModule = PyImport_ImportModule(AppName.c_str()); // new reference

		// check if error occurred
		if (!PyErr_Occurred() && pModule != nullptr)
			regularMode = true;

		else
			errorMode = true;
	}

}