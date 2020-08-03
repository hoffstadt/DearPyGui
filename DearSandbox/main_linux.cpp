#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "mvStdOutput.h"
#include "mvMarvel.h"
#include "mvApp.h"
#include <iostream>
#include <fstream>
#include <filesystem>


namespace fs = std::filesystem;
using namespace Marvel;

int main(int argc, char* argv[])
{
	mvApp::GetApp()->setArgv0(argv[0]);

	PyObject* m = nullptr;

	// initialize python
	// add our custom module
	PyImport_AppendInittab("sandboxout", &PyInit_embOut);
	PyImport_AppendInittab("dearpygui", &PyInit_dearpygui);

	// set path and start the interpreter
	std::string pathpath = ":../../DearSandbox";
	wchar_t* deco = Py_DecodeLocale(pathpath.c_str(), nullptr);

	wchar_t* oldpath = Py_GetPath();

	std::wstring fuck = std::wstring(oldpath) + deco;
    Py_SetPath(fuck.c_str());
	Py_NoSiteFlag = 1; // this must be set to 1
	Py_DontWriteBytecodeFlag = 1;
	//_PyEval_SetSwitchInterval()
	
	Py_Initialize();
	if (!Py_IsInitialized())
	{
		printf("Error initializing Python interpreter\n");
		return 1;
	}

	PyObject* mmarvel = PyImport_ImportModule("dearpygui");

	// import our custom module to capture stdout/stderr
	m = PyImport_ImportModule("sandboxout");
	PySys_SetObject("stdout", m);
	PySys_SetObject("stderr", m);

	PyObject* pModule = PyImport_ImportModule("Demo"); // new reference

	// check if error occurred
	if (!PyErr_Occurred() && pModule != nullptr)
	{
		Py_XDECREF(pModule);
		Py_XDECREF(mmarvel);
		return 0;
	}

	else
	{
		start_dearpygui_error();
		return 0;
	}
	
}