#pragma once

#include "Core/mvPythonModule.h"

namespace Marvel {

	void CreateLoggerInterface(mvPythonModule& pyModule, PyObject* (*initfunc)());

}