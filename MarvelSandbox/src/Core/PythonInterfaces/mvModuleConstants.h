#pragma once

#include <vector>
#include <string>

namespace Marvel {

	class mvPythonModule;

	extern std::vector<std::pair<std::string, long>> ModuleConstants;

	void CreateConstantsInterface(mvPythonModule& pyModule, PyObject* (*initfunc)());

}