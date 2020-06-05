#pragma once

#include "Core/mvPythonModule.h"

namespace Marvel {

	void CreatePlotInterface(mvPythonModule& pyModule, PyObject* (*initfunc)());

}