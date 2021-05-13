#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "mvModule_Core.h"
#include "mvViewport.h"
#include <iostream>

using namespace Marvel;

int main(int argc, char* argv[])
{

	PyObject* m = nullptr;

	// initialize python
	// add our custom module
	PyImport_AppendInittab("core", &PyInit_core);

	// set path and start the interpreter
#if defined(__APPLE__)
  wchar_t *deco = Py_DecodeLocale(
      PYTHON_LIBS_PATH":"
      PYTHON_LIBS_PATH"/lib-dynload:"
      PYTHON_LIBS_PATH"/site-packages:"
      "../../DearPyGui:"
      "../../DearSandbox",
      nullptr);
#else
  wchar_t *deco = Py_DecodeLocale(
      "../../Dependencies/cpython/debug/build/lib.linux-x86_64-3.9-pydebug/:../"
      "../Dependencies/cpython/Lib/:../../DearPyGui:../../DearSandbox",
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

	PyObject* mmarvel = PyImport_ImportModule("core");

	PyObject* pModule = PyImport_ImportModule("sandbox"); // new reference

	// check if error occurred
	if (!PyErr_Occurred() && pModule != nullptr)
	{
		Py_XDECREF(pModule);
		Py_XDECREF(mmarvel);
		return 0;
	}

	PyErr_Print();

    return 0;
}