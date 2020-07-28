#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <frameobject.h>
#include <string>
#include "mvPythonTranslator.h"

namespace Marvel
{

	static void ThrowPythonException(const std::string& message)
	{

		std::string fullMessage = "Line: %d \t" + message;

		mvGlobalIntepreterLock gil;

		int line = PyFrame_GetLineNumber(PyEval_GetFrame());
		PyObject* ex = PyErr_Format(PyExc_Exception,
			fullMessage.c_str(), line);
		PyErr_Print();
	}

}