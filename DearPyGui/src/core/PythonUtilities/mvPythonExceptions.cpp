#include "mvPythonExceptions.h"
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <frameobject.h>
#include <string>
#include "mvApp.h"
#include "mvViewport.h"
#include "mvCallbackRegistry.h"
#include "mvGlobalIntepreterLock.h"

namespace Marvel
{

	void mvThrowPythonError(int code, const std::string& message)
	{

		if (mvApp::GetApp()->checkIfMainThread())
		{
			std::string fullMessage = "Error: [%d] Message: \t" + message;
			PyErr_Format(PyExc_Exception, fullMessage.c_str(), code);
		}
			
	}

}
