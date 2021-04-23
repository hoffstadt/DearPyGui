#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "mvModule_Core.h"
#include "mvApp.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "mvViewport.h"
#include "mvLog.h"

namespace fs = std::filesystem;
using namespace Marvel;

int main(int argc, char* argv[])
{

#ifdef MV_RELEASE
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_SHOW);
	mvPythonParser::GenerateStubFile("../../DearPyGui/dearpygui/core.pyi");
#else
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_SHOW);	
#endif

	// add our custom module
	//PyImport_AppendInittab("sandboxout", &PyInit_embOut);
	PyImport_AppendInittab("core", &PyInit_core);

	// set path and start the interpreter
	wchar_t* path = Py_DecodeLocale("../../DearSandbox/;../../Dependencies/cpython/Lib;../../Dependencies/cpython/PCbuild/amd64;../../DearPyGui/", nullptr);

	Py_SetPath(path);
	Py_NoSiteFlag = 1; // this must be set to 1
	Py_DontWriteBytecodeFlag = 1;
	
	// initialize python
	Py_Initialize();
	if (!Py_IsInitialized())
	{
		printf("Error initializing Python interpreter\n");
		return 1;
	}

	PyObject* mmarvel = PyImport_ImportModule("core");

	PyObject* pModule = PyImport_ImportModule("sandbox"); // new reference

	// check if error occurred
	if (!PyErr_Occurred() && pModule != nullptr)
	{
		Py_XDECREF(pModule);
		Py_XDECREF(mmarvel);
	}

	else
		PyErr_Print();
	
}