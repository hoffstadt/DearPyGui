#include "mvPython.h"
#include "mvStdOutput.h"
#include "mvModule_Core.h"
#include "mvApp.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "mvWindow.h"
#include "mvLog.h"

namespace fs = std::filesystem;
using namespace Marvel;

int main(int argc, char* argv[])
{

#ifdef MV_RELEASE
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);
	GenerateStubFile("../../DearPyGui/dearpygui/core.pyi");
#else
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_SHOW);	
#endif

	// add our custom module
	PyImport_AppendInittab("sandboxout", &PyInit_embOut);
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

	// import our custom module to capture stdout/stderr
	PyObject* m = PyImport_ImportModule("sandboxout");
	PySys_SetObject("stdout", m);
	PySys_SetObject("stderr", m);

	PyObject* pModule = PyImport_ImportModule("sandbox"); // new reference

	// check if error occurred
	if (!PyErr_Occurred() && pModule != nullptr)
	{
		Py_XDECREF(pModule);
		Py_XDECREF(mmarvel);
	}

	else
	{
		PyErr_Print();

		// create window
		auto window = mvWindow::CreatemvWindow(mvApp::GetApp()->getActualWidth(), mvApp::GetApp()->getActualHeight(), true);
		window->show();
		window->run();
		delete window;
		delete mvApp::GetApp();
	}
	
}