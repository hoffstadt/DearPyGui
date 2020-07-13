# pragma once

#include "mvPythonTranslator.h"
#include <map>
#include <string>

namespace Marvel {

	class mvPythonModule;

	using pyInitFunc = mvPythonModule * (*)();
	using pyDocFunc = std::map<std::string, mvPythonParser>&(*)();

	std::vector<std::pair<std::string, long>> BuildConstantsInterface();

	std::map<std::string, mvPythonParser>& BuildAppInterface();
	std::map<std::string, mvPythonParser>& BuildWidgetsInterface();
	std::map<std::string, mvPythonParser>& BuildDrawingInterface();
	std::map<std::string, mvPythonParser>& BuildInputsInterface();
	std::map<std::string, mvPythonParser>& BuildLoggingInterface();
	std::map<std::string, mvPythonParser>& BuildPlottingInterface();

	mvPythonModule* CreatePythonInterface      ();
	mvPythonModule* CreateInputInterface       ();
	mvPythonModule* CreateLoggerInterface      ();
	mvPythonModule* CreatePlotInterface        ();
	mvPythonModule* CreateDrawingInterface     ();
	mvPythonModule* CreateWidgetAddingInterface();
	mvPythonModule* CreateConstantsInterface   ();

}