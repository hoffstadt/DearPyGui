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

		std::string fullMessage = "Error: [%d] Message: \t" + message;
		PyErr_Format(PyExc_Exception, fullMessage.c_str(), 1000);

	}

}
