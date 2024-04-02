#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "dearpygui.h"
#include "mvViewport.h"
#include <fstream>
#include <string>
#include <sstream>

void runTest(std::string test)
{
	auto ss = std::ostringstream{};
	std::ifstream input_file("../../testing/" + test + ".py");
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

	GenerateStubFile("../../dearpygui");
	GenerateDearPyGuiFile("../../dearpygui");
	GenerateDearPyGuiFileRTD("../../dearpygui");

	// initialize python
	// add our custom module
	PyImport_AppendInittab("_dearpygui", &PyInit__dearpygui);

	// set path and start the interpreter
#if defined(__APPLE__)
  wchar_t *deco = Py_DecodeLocale(
      PYTHON_LIBS_PATH":"
      PYTHON_LIBS_PATH"/lib-dynload:"
      PYTHON_LIBS_PATH"/site-packages:"
      "../..:"
      "../../thirdparty/DearPyGui_Ext:"
      "../../sandbox",
      nullptr);
#else
  wchar_t *deco = Py_DecodeLocale(
      "../../thirdparty/DearPyGui_Ext/:../../thirdparty/cpython/build/debug/build/lib.linux-x86_64-3.9-pydebug/:../"
      "../thirdparty/cpython/Lib/:../..:../../sandbox",
      nullptr);
#endif

    Py_SetPath(deco);
	Py_NoSiteFlag = 1; // this must be set to 1
	Py_DontWriteBytecodeFlag = 1;
	//_PyEval_SetSwitchInterval()
	
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
		std::ifstream input_file("../../sandbox/sandbox.py");
		ss << input_file.rdbuf();
		PyRun_SimpleString(ss.str().c_str());
	}
#endif

	// check if error occurred
	if (!PyErr_Occurred())
		Py_XDECREF(mmarvel);
	else
		PyErr_Print();

    return 0;
}