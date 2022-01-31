#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "dearpygui.h"
#include "mvContext.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>
#include "mvViewport.h"
#include "mvLog.h"

namespace fs = std::filesystem;

void runTest(std::string test)
{
	auto ss = std::ostringstream{};
	std::ifstream input_file("../../Tests/" + test + ".py");
#ifndef MV_TESTS_ONLY
	ss << "should_exit = False\n";
#else
	ss << "should_exit = True\n";
#endif
	ss << input_file.rdbuf();
	PyRun_SimpleString(ss.str().c_str());
}

int main(int argc, char* argv[])
{

	GenerateStubFile("../../DearPyGui/dearpygui");
	GenerateDearPyGuiFile("../../DearPyGui/dearpygui");
	GenerateDearPyGuiFileRTD("../../DearPyGui/dearpygui");

#ifdef MV_RELEASE
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_SHOW);
#else
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_SHOW);	
#endif

	// add our custom module
	PyImport_AppendInittab("_dearpygui", &PyInit__dearpygui);

	// set path and start the interpreter
	wchar_t* path = Py_DecodeLocale("../../Dependencies/DearPyGui_Ext/;../../DearSandbox/;../../Dependencies/cpython/Lib;../../Dependencies/cpython/PCbuild/amd64;../../DearPyGui/", nullptr);

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

	PyObject* mmarvel = PyImport_ImportModule("_dearpygui");

	// tests
	runTest("simple_tests");

#ifndef MV_TESTS_ONLY
	// sandbox
	{
		auto ss = std::ostringstream{};
		std::ifstream input_file("../../DearSandbox/sandbox.py");
		ss << input_file.rdbuf();
		int result = PyRun_SimpleString(ss.str().c_str());
	}
#endif

	// check if error occurred
	if (!PyErr_Occurred())
		Py_XDECREF(mmarvel);
	else
		PyErr_Print();
}