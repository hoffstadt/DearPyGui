#pragma once
#include "mvCore.h"

// extremely nasty TEMPORARY code until
// the python c API can be properly
// seperated from the architecture.
// May the Clean Code Gods forgive me.

#ifdef MV_CPP
#include <iostream>
#include "CppUtilities/mvCppPyObject.h"
#define PyGILState_STATE int
#define PyExc_Exception -1
#define PyMODINIT_FUNC int
#define Py_None nullptr
#define Py_XINCREF(x) x
#define Py_XDECREF(x) x
#define Py_BEGIN_ALLOW_THREADS //
#define Py_END_ALLOW_THREADS //
#define ThrowPythonException(x) std::cout<<x<<std::endl

inline int       PyEval_GetFrame() { return -1; }
inline int       PyFrame_GetLineNumber(int) { return -1; }
inline void      PyErr_Format(int, const char*, int, int line = 0) {}
inline void      PyErr_Format(int, const char*, const char*, int line = 0) {}
inline void      PyErr_Print() { std::cout << "Error" << std::endl; };
inline PyObject* PyTuple_New(int) { return nullptr; }
inline PyObject* PyDict_New() { return nullptr; }
inline PyObject* PyTuple_GetItem(PyObject*, int) { return nullptr; }
inline void      PyTuple_SetItem(PyObject*, int, PyObject*) {}
inline PyObject* PyFloat_FromDouble(double x) { return nullptr; }
inline PyObject* PyLong_FromLong(int x) { return nullptr; }
inline bool      PyList_Check(PyObject*) { return true; }


#include "CppUtilities/mvCppParser.h"
#include "CppUtilities/mvCppTranslator.h"

#else
	class PyContextManager {};
	#define PY_SSIZE_T_CLEAN
	#include <Python.h>
	#include <frameobject.h>
	#include "mvPyObject.h"
	#include "mvDataStorage.h"
	#include "mvPythonExceptions.h"
	#include "mvPythonParser.h"
	#include "mvPythonTranslator.h"
	#include "mvGlobalIntepreterLock.h"
#endif
