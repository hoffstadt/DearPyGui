#include "Core/mvPythonModule.h"
#include "Core/PythonInterfaces/mvStdOutput.h"
#include "Core/mvWindow.h"
#include "Platform/Windows/mvWindowsWindow.h"
#include <iostream>
#include "Core/PythonInterfaces/mvModuleConstants.h"

using namespace Marvel;

AppLog* AppLog::s_instance = nullptr;

// declare our modules
MV_DECLARE_PYMODULE(pyMod1, "sbApp", {});
MV_DECLARE_PYMODULE(pyMod2, "sbInput", {});
MV_DECLARE_PYMODULE(pyMod3, "sbLog", {});
MV_DECLARE_PYMODULE(pyMod4, "sbPlot", {});
MV_DECLARE_PYMODULE(pyMod5, "sbDraw", {});
MV_DECLARE_PYMODULE(pyMod6, "sbWidgets", {});
MV_DECLARE_PYMODULE(pyMod7, "sbConstants", ModuleConstants);

namespace Marvel {
	extern void CreatePythonInterface(mvPythonModule& pyModule, PyObject* (*initfunc)());
	extern void CreateInputInterface(mvPythonModule& pyModule, PyObject* (*initfunc)());
	extern void CreateLoggerInterface(mvPythonModule& pyModule, PyObject* (*initfunc)());
	extern void CreatePlotInterface(mvPythonModule& pyModule, PyObject* (*initfunc)());
	extern void CreateDrawingInterface(mvPythonModule& pyModule, PyObject* (*initfunc)());
	extern void CreateWidgetAddingInterface(mvPythonModule& pyModule, PyObject* (*initfunc)());
}

int main(int argc, char* argv[])
{

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

	const wchar_t* path;
	const char* module_name;
	
#ifdef MV_RELEASE
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);
#else
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_SHOW);
#endif // MV_RELEASE

	// create window
	mvWindow* window = new mvWindowsWindow();
	window->show();

	// get path
	if(argc < 2)
		path = L"python38.zip;../../MarvelSandbox";
	else if(argc == 2)
		path = L"python38.zip;";
	else
	{
		wchar_t* deco = Py_DecodeLocale(argv[1], nullptr);
		std::wstring* wpath = new std::wstring(std::wstring(deco) + std::wstring(L";python38.zip"));
		path = wpath->c_str();
	}

	// add our custom module
	PyImport_AppendInittab("sandboxout", &PyInit_embOut);

	// set path and start the interpreter
	Py_SetPath(path);
	Py_NoSiteFlag = 1; // this must be set to 1

	Py_Initialize();
	PyGILState_STATE gstate = PyGILState_Ensure();

	if (!Py_IsInitialized())
	{
		printf("Error initializing Python interpreter\n");
		return 1;
	}

	// import our custom module to capture stdout/stderr
	PyObject* m = PyImport_ImportModule("sandboxout");
	PySys_SetObject("stdout", m);
	PySys_SetObject("stderr", m);

	// get module name
	if (argc < 2)
		module_name = "App";
	else if (argc == 2)
		module_name = argv[1];
	else
		module_name = argv[2];
	
	// info
	AppLog::getLogger()->AddLog("[Sandbox Version] %0s\n", mvApp::getVersion());
	AppLog::getLogger()->AddLog("[Python Version] %0s\n", PY_VERSION);
	AppLog::getLogger()->AddLog("[ImGui Version] %0s\n", IMGUI_VERSION);
	AppLog::getLogger()->AddLog("[Compiler] %0s\n", Py_GetCompiler());

	// get module
	PyObject* pModule = PyImport_ImportModule(module_name); // new reference

	// check if error occurred
	if (!PyErr_Occurred() && pModule != nullptr)
	{
		// returns the dictionary object representing the module namespace
		PyObject* pDict = PyModule_GetDict(pModule); // borrowed reference
		mvApp::GetApp()->setModuleDict(pDict);
		mvApp::GetApp()->setStarted();
		window->run();
		Py_XDECREF(pModule);

	}
	else
	{
		PyErr_Print();
		mvApp::GetApp()->setOk(false);
		mvApp::GetApp()->showLogger();
		window->run();
	}

	Py_XDECREF(m);

	PyMem_RawFree(program);

	// shutdown the interpreter
	if (Py_FinalizeEx() < 0)
		exit(120);

	/* Release the thread. No Python API allowed beyond this point. */
	PyGILState_Release(gstate);

}