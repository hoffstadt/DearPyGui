#include "include/MarvelApplication.h"
#include "mvPythonModule.h"
#include "Core/PythonInterfaces/mvStdOutput.h"
#include "include/mvModuleInitializer.h"
#include "Core/mvWindow.h"
#include "Core/StandardWindows/mvAppEditor.h"
#include "Platform/Windows/mvWindowsWindow.h"
#include <iostream>
#include <fstream>
#include "mvInterfaceRegistry.h"
#include <CLI11.hpp>
#include <iostream>
#include "Core/StandardWindows/mvSourceWindow.h"
#include <nlohmann/json.hpp>

#include <filesystem>

namespace fs = std::filesystem;

namespace Marvel {

	mvAppLog* mvAppLog::s_instance = nullptr;
	const char* Application::s_argv0 = nullptr;
	thread_local mvWorkStealingQueue* mvThreadPool::m_local_work_queue;
	thread_local unsigned mvThreadPool::m_index;

	Application::Application(const char* name, int argc, char* argv[]) : argc(argc), argv(argv)
	{
		static_cast<mvAppEditor*>(mvAppEditor::GetAppEditor())->setProgramName(name);
		s_argv0 = argv[0];
	}

	Application::~Application()
	{
		auto m_gstate = PyGILState_Ensure();
		Py_XDECREF(m);

		PyMem_RawFree(program);

		PyGILState_Release(m_gstate);

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
		bool ignoreConfig = false;

		CLI::App app{ "Marvel Sandbox" };
		app.allow_windows_style_options();

		// options
		app.add_option("-a, --app", AppName, "Name of the python file (without extension)");
		app.add_option("-p, --path", PathName, "Path to app file (default is location of MarvelSandbox.exe)");
		app.add_option("-l, --libs", LibraryPath, "Path to 3rd party python libraries");

		// flags
		app.add_flag("-d, --documentation", documentation, "Sets MarvelSandbox to Documentation Mode");
		app.add_flag("-e, --editor", editorMode, "Sets MarvelSandbox to Editor Mode");
		app.add_flag("-n, --noconfig", ignoreConfig, "Ignores config file");

		CLI11_PARSE(app, argc, argv);

		// no commandline arguments, so use config file
		if (!ignoreConfig)
		{
			std::ifstream stream("marvel_config.json");
			if (stream.good())
			{
				nlohmann::json j = nlohmann::json::parse(stream);

				if (j.contains("App") && AppName.empty())
				{
					AppName = j["App"];
				}
				else if(AppName.empty())
					AppName = "Demo";

				if (j.contains("Mode"))
				{

					std::string mode = j["Mode"];
					if (mode == "Documentation")
						documentation = true;
					else if (mode == "Editor")
						editorMode = true;
				}

				if (j.contains("Theme"))
					theme = j["Theme"];

				if (j.contains("Path"))
					PathName = PathName + std::string(j["Path"]);

				if (j.contains("PythonLibs"))
					LibraryPath = j["PythonLibs"];


				if (j.contains("Development"))
					ranFromVS = j["Development"];
			}

			return 0;
		}


		return 0;

	}

	void Application::handlePaths()
	{
		fs::path p = fs::path(argv[0]);
		p.replace_extension(" ");
		auto dependencies = p.parent_path().string() + "/Dependencies;";
		auto excpath = p.parent_path().string() + ";";

		addedPath = PathName + "\\";

		PathName = excpath + dependencies + PathName + ";Dependencies/python38.zip;" + LibraryPath + ";";

		program = Py_DecodeLocale(argv[0], NULL);
		if (program == NULL) {
			fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
			exit(1);
		}
		Py_SetProgramName(program);  /* optional but recommended */

		if(ranFromVS)
			addedPath = std::string(MV_MAIN_DIR);
	
	}

	int Application::initializePython()
	{
		// add our custom module
		PyImport_AppendInittab("sandboxout", &PyInit_embOut);

		// set path and start the interpreter
		wchar_t* deco = Py_DecodeLocale(PathName.c_str(), nullptr);
		Py_SetPath(deco);
		Py_NoSiteFlag = 1; // this must be set to 1
		Py_DontWriteBytecodeFlag = 1;

		Py_Initialize();
		if (!Py_IsInitialized())
		{
			printf("Error initializing Python interpreter\n");
			return 1;
		}

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
		mvApp::GetApp()->setFile(filename);
		PyEval_SaveThread(); // releases global lock
		mvApp::GetApp()->precheck();
		mvApp::SetAppStarted();

		// create window
		mvWindow* window = new mvWindowsWindow(mvApp::GetApp()->getActualWidth(), mvApp::GetApp()->getActualHeight());
		window->show();
		mvApp::GetApp()->setAppTheme(theme);
		window->run();
		PyGILState_STATE gstate = PyGILState_Ensure();
		Py_XDECREF(pModule);
		delete window;
		delete mvApp::GetApp();

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
		mvApp::GetApp()->setMainThreadID(std::this_thread::get_id());

		// get module
		pModule = PyImport_ImportModule(AppName.c_str()); // new reference

		// check if error occurred
		if (!PyErr_Occurred() && pModule != nullptr)
			regularMode = true;

		else
			errorMode = true;
	}

	void Application::setLogLevel(unsigned level)
	{
		mvAppLog::getLogger()->setLogLevel(level);
	}

	void Application::Log(const std::string& text, const std::string& level)
	{
		mvAppLog::getLogger()->Log(text, level);
	}

	void Application::LogDebug(const std::string& text)
	{
		mvAppLog::getLogger()->LogDebug(text);
	}

	void Application::LogInfo(const std::string& text)
	{
		mvAppLog::getLogger()->LogInfo(text);
	}

	void Application::LogWarning(const std::string& text)
	{
		mvAppLog::getLogger()->LogWarning(text);
	}

	void Application::LogError(const std::string& text)
	{
		mvAppLog::getLogger()->LogError(text);
	}

	void Application::ClearLog()
	{
		mvAppLog::getLogger()->ClearLog();
	}
}