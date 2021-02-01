#include "mvPython.h"
#include <string>

namespace Marvel
{

	void ThrowPythonException(const std::string& message, bool line)
	{

		std::string fullMessage = "Line: %d \t" + message;

		int lineno = 0;
		if (line)
		{
			lineno = PyFrame_GetLineNumber(PyEval_GetFrame());
		}

		PyErr_Format(PyExc_Exception, fullMessage.c_str(), lineno);
		PyErr_Print();
	}

}
