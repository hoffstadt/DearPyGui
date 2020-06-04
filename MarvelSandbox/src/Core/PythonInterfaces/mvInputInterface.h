#pragma once

#include "Core/mvPythonModule.h"

namespace Marvel {
	
	void CreateInputInterface(mvPythonModule& pyModule, PyObject*(*initfunc)());

}