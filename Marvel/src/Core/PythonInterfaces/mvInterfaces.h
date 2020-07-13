# pragma once

#include "mvPythonParser.h"
#include <map>
#include <string>

namespace Marvel {

	class mvPythonModule;

	using pyInitFunc = mvPythonModule * (*)();
	using pyDocFunc = std::map<std::string, mvPythonParser>&(*)();

	std::vector<std::pair<std::string, long>> BuildConstantsInterface();
	std::map<std::string, mvPythonParser>&    BuildMarvelInterface();

	mvPythonModule* CreateMarvelInterface      ();
	mvPythonModule* CreateConstantsInterface   ();

}