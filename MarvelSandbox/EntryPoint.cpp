#include "Core/PythonUtilities/mvPythonModule.h"
#include "Core/PythonInterfaces/mvStdOutput.h"
//#include "Core/Concurrency/mvThreadPool.h"
#include "Core/mvWindow.h"
#include "Core/mvAppEditor.h"
#include "Platform/Windows/mvWindowsWindow.h"
#include <iostream>
#include <fstream>
#include "Core/PythonInterfaces/mvInterfaceRegistry.h"
#include "Core/PythonInterfaces/mvInterfaces.h"
#include <CLI11.hpp>
#include <filesystem>

namespace fs = std::filesystem;
using namespace Marvel;

mvAppLog* mvAppLog::s_instance = nullptr;

// declare our modules
MV_DECLARE_PYMODULE(pyMod1, "sbApp", {});
MV_DECLARE_PYMODULE(pyMod2, "sbInput", {});
MV_DECLARE_PYMODULE(pyMod3, "sbLog", {});
MV_DECLARE_PYMODULE(pyMod4, "sbPlot", {});
MV_DECLARE_PYMODULE(pyMod5, "sbDraw", {});
MV_DECLARE_PYMODULE(pyMod6, "sbWidgets", {});
MV_DECLARE_PYMODULE(pyMod7, "sbConstants", mvInterfaceRegistry::GetRegistry()->getConstants());

bool doesFileExists(const char* filepath, const char** modname = nullptr);

int main(int argc, char* argv[])
{

#ifdef MV_RELEASE
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);
#else
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_SHOW);
#endif

	CLI::App app{ "Marvel Sandbox" };
	app.allow_windows_style_options();

	// options
	std::string AppName = "App";
	std::string PathName = "";
	app.add_option("-a, --app", AppName, "Name of the python file (without extension)");
	app.add_option("-p, --path", PathName, "Path to app file (default is location of MarvelSandbox.exe)");

	// flags
	bool logger = false;
	bool metrics = false;
	bool source = false;
	bool documentation = false;
	bool editorMode = true;
	bool editorMode2 = false;
	app.add_flag("-l, --logger", logger, "Show Logger");
	app.add_flag("-m, --metrics", metrics, "Show Metrics");
	app.add_flag("-s, --source", source, "Show Source");
	app.add_flag("-d, --documentation", documentation, "Show Documentation");
	app.add_flag("-e, --editor", editorMode, "Sets MarvelSandbox to Editor Mode");
	app.add_flag("-o, --editorOff", editorMode2, "Sets MarvelSandbox to Editor Mode Off");

	CLI11_PARSE(app, argc, argv);

	std::string addedpath = PathName;

	if (editorMode2)
		editorMode = false;

	if (editorMode)
	{
		mvWindow* window = new mvWindowsWindow(mvAppEditor::GetAppEditor()->getWindowWidth(), mvAppEditor::GetAppEditor()->getWindowHeight(), true);
		window->show();
		window->run();

		return 0;
	}

	PathName = PathName + ";python38.zip";

	wchar_t* program = Py_DecodeLocale(argv[0], NULL);
	if (program == NULL) {
		fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
		exit(1);
	}
	Py_SetProgramName(program);  /* optional but recommended */

	// initialize our modules
	MV_INIT_PYMODULE(pyMod1, CreatePythonInterface);
	MV_INIT_PYMODULE(pyMod2, CreateInputInterface);
	MV_INIT_PYMODULE(pyMod3, CreateLoggerInterface);
	MV_INIT_PYMODULE(pyMod4, CreatePlotInterface);
	MV_INIT_PYMODULE(pyMod5, CreateDrawingInterface);
	MV_INIT_PYMODULE(pyMod6, CreateWidgetAddingInterface);
	MV_INIT_PYMODULE(pyMod7, CreateConstantsInterface);

	const char* module_name = nullptr;
	doesFileExists("SandboxConfig.txt", &module_name);

	// get module name and path
	if (module_name) // ran with config file
	{
		AppName = module_name;
		PathName = "python38.zip;";
	}

	if (argc < 2) // ran from visual studio
	{
		//addedpath = std::string(MV_MAIN_DIR) + std::string("MarvelSandbox/");
		PathName = "python38.zip;../../MarvelSandbox";
	}

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
	PyObject* m = PyImport_ImportModule("sandboxout");
	PySys_SetObject("stdout", m);
	PySys_SetObject("stderr", m);

	// info
	mvAppLog::getLogger()->AddLog("[Sandbox Version] %0s\n", mvApp::getVersion());
	mvAppLog::getLogger()->AddLog("[Python Version] %0s\n", PY_VERSION);
	mvAppLog::getLogger()->AddLog("[ImGui Version] %0s\n", IMGUI_VERSION);
	mvAppLog::getLogger()->AddLog("[Compiler] MSVC version %0d\n", _MSC_VER);

	// get module
	PyObject* pModule = PyImport_ImportModule(AppName.c_str()); // new reference
	
	// check if error occurred
	if (!PyErr_Occurred() && pModule != nullptr)
	{
		// returns the dictionary object representing the module namespace
		PyObject* pDict = PyModule_GetDict(pModule); // borrowed reference
		mvApp::GetApp()->setModuleDict(pDict);
		std::string filename = addedpath + "\\" + std::string(AppName) + ".py";
		mvApp::GetApp()->setFile(filename);
		PyEval_SaveThread(); // releases global lock
		mvApp::GetApp()->preRender();
		mvApp::GetApp()->setStarted();

		if(logger) mvApp::GetApp()->showLogger();
		if(source) mvApp::GetApp()->showSource();
		if(metrics) mvApp::GetApp()->showMetrics();
		if(documentation) mvApp::GetApp()->showDoc();

		// create window
		mvWindow* window = new mvWindowsWindow(mvApp::GetApp()->getWindowWidth(), mvApp::GetApp()->getWindowHeight());
		window->show();

		window->run();
		PyGILState_STATE gstate = PyGILState_Ensure();
		Py_XDECREF(pModule);

	}
	else
	{
		PyErr_Print();
		mvApp::GetApp()->setOk(false);
		mvApp::GetApp()->showLogger();

		// create window
		mvWindow* window = new mvWindowsWindow(mvApp::GetApp()->getWindowWidth(), mvApp::GetApp()->getWindowHeight());
		window->show();

		window->run();
	}

	Py_XDECREF(m);

	PyMem_RawFree(program);

	// shutdown the interpreter
	if (Py_FinalizeEx() < 0)
		exit(120);

}

bool doesFileExists(const char* filepath, const char** modname)
{
	/* try to open file to read */
	std::ifstream ifile;
	ifile.open(filepath, std::ios::in);
	if (ifile)
	{
		std::string line;
		std::getline(ifile, line);
		auto pline = new std::string(line);
		*modname = pline->data();
		return true;
	}
	return false;
}