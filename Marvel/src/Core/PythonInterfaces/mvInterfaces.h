# pragma once

#include "mvPythonTranslator.h"
#include <map>
#include <string>

namespace Marvel {

	class mvPythonModule;

	using pyInitFunc = mvPythonModule * (*)();
	using pyDocFunc = std::map<std::string, mvPythonTranslator>&(*)();

	std::vector<std::pair<std::string, long>> BuildConstantsInterface();

	std::map<std::string, mvPythonTranslator>& BuildAppInterface();
	std::map<std::string, mvPythonTranslator>& BuildWidgetsInterface();
	std::map<std::string, mvPythonTranslator>& BuildDrawingInterface();
	std::map<std::string, mvPythonTranslator>& BuildInputsInterface();
	std::map<std::string, mvPythonTranslator>& BuildLoggingInterface();
	std::map<std::string, mvPythonTranslator>& BuildPlottingInterface();

	mvPythonModule* CreatePythonInterface      ();
	mvPythonModule* CreateInputInterface       ();
	mvPythonModule* CreateLoggerInterface      ();
	mvPythonModule* CreatePlotInterface        ();
	mvPythonModule* CreateDrawingInterface     ();
	mvPythonModule* CreateWidgetAddingInterface();
	mvPythonModule* CreateConstantsInterface   ();

}