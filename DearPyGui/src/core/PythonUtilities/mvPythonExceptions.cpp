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
			auto f = PyEval_GetFrame();
			if (f)
				lineno = PyFrame_GetLineNumber(f);
			else
				fullMessage.append(" || error occured in a callback");
		}

		PyErr_Format(PyExc_Exception, fullMessage.c_str(), lineno);
		PyErr_Print();
	}

	void mvThrowPythonError(int code, const std::string& message, bool line)
	{
		std::string fullMessage = "Error: [" + std::to_string(code) + std::string("] Line: %d \t") + message;

		int lineno = 0;
		if (line)
		{
			auto f = PyEval_GetFrame();
			if (f)
				lineno = PyFrame_GetLineNumber(f);
			else
				fullMessage.append(" || error occured in a callback");
		}

		PyErr_Format(PyExc_Exception, fullMessage.c_str(), lineno);
		PyErr_Print();
	}

}
