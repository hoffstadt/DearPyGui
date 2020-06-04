#pragma once

#include "mvPythonModule.h"

namespace Marvel {
	
	void CreatePythonInterface(mvPythonModule& pyModule, PyObject*(*initfunc)());

}