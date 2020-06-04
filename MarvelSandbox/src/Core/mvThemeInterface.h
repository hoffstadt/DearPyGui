#pragma once

#include "mvPythonModule.h"

namespace Marvel {

	void CreateThemeInterface(mvPythonModule& pyModule, PyObject* (*initfunc)());

}