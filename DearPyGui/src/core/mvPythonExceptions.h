#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <frameobject.h>
#include <string>
#include "mvPythonTranslator.h"

namespace Marvel
{

	void ThrowPythonException(const std::string& message);

}
