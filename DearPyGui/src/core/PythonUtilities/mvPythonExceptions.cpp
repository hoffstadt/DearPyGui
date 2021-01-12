#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <frameobject.h>
#include <string>
#include "mvPythonTranslator.h"
#include "mvGlobalIntepreterLock.h"

namespace Marvel
{

	void ThrowPythonException(const std::string& message)
	{

		std::string fullMessage = "Line: %d \t" + message;

		 

		int line = PyFrame_GetLineNumber(PyEval_GetFrame());
		PyErr_Format(PyExc_Exception, fullMessage.c_str(), line);
		PyErr_Print();
	}

}
