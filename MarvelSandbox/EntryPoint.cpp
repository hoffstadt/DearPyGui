#define MV_SANDBOX_VERSION "0.0.1"

#include "Core/PythonInterfaces/mvAppInterface.h"
#include "Core/PythonInterfaces/mvInputInterface.h"
#include "Core/PythonInterfaces/mvLoggerInterface.h"
#include "Core/PythonInterfaces/mvStdOutput.h"
#include "Core/mvWindow.h"
#include "Platform/Windows/mvWindowsWindow.h"
#include <iostream>

using namespace Marvel;

AppLog* AppLog::s_instance = nullptr;

// declare our modules
MV_DECLARE_PYMODULE(pyMod1, "sbApp");
MV_DECLARE_PYMODULE(pyMod2, "sbInput");
MV_DECLARE_PYMODULE(pyMod3, "sbLog");

int main(int argc, char* argv[])
{

	// initialize our modules
	MV_INIT_PYMODULE(pyMod1, CreatePythonInterface);
	MV_INIT_PYMODULE(pyMod2, CreateInputInterface);
	MV_INIT_PYMODULE(pyMod3, CreateLoggerInterface);

	const wchar_t* path;
	const char* module_name;
	
#ifdef MV_RELEASE
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);
#endif // MV_RELEASE

	mvWindow* window = new mvWindowsWindow();
	window->show();

	// get path
	if(argc < 2)
		path = L"python38.zip;../../../../MarvelSandbox";
	else if(argc == 2)
		path = L"python38.zip;";
	else
	{
		// Convert to a wchar_t*
		size_t origsize = strlen(argv[1]) + 1;
		const size_t newsize = 100;
		size_t convertedChars = 0;
		wchar_t wcstring[newsize];
		mbstowcs_s(&convertedChars, wcstring, origsize, argv[1], _TRUNCATE);
		std::wstring wpath = std::wstring(L"python38.zip;") + std::wstring(wcstring);
		path = wpath.c_str();
	}
	
	// add our custom module
	PyImport_AppendInittab("sandboxout", &PyInit_embOut);

	// set path and start the intpreter
	Py_SetPath(path);
	Py_Initialize();

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
	AppLog::getLogger()->AddLog("[Sandbox Version] %0s\n", MV_SANDBOX_VERSION);
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
		window->run();
		Py_XDECREF(pModule);

	}
	else
	{
		PyErr_Print();
		mvApp::GetApp()->setOk(false);
		window->run();
	}

	Py_XDECREF(m);

	// shutdown the interpreter
	Py_Finalize();

}