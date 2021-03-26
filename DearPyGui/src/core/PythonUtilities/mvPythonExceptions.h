#pragma once

#include "mvPython.h"
#include <string>

namespace Marvel
{

	// older command
	void ThrowPythonException(const std::string& message, bool line = true);

	// new command
	void mvThrowPythonError(int code, const std::string& message, bool line = true);

}
