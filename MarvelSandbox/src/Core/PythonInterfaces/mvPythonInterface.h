#pragma once

#include "Core/mvPythonModule.h"

namespace Marvel {
	
	void CreatePythonInterface(mvPythonModule& pyModule, PyObject*(*initfunc)());

}