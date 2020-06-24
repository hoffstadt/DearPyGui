# pragma once

#include "Core/PythonUtilities/mvPythonTranslator.h"
#include "mvInterfaceRegistry.h"
#include <map>
#include <string>

namespace Marvel {

	class mvPythonModule;

	std::vector<std::pair<std::string, long>> BuildConstantsInterface();

	std::map<std::string, mvPythonTranslator> BuildAppInterface();
	std::map<std::string, mvPythonTranslator> BuildWidgetsInterface();
	std::map<std::string, mvPythonTranslator> BuildDrawingInterface();
	std::map<std::string, mvPythonTranslator> BuildInputsInterface();
	std::map<std::string, mvPythonTranslator> BuildLoggingInterface();
	std::map<std::string, mvPythonTranslator> BuildPlottingInterface();

	void CreatePythonInterface      (mvPythonModule& pyModule, PyObject* (*initfunc)());
	void CreateInputInterface       (mvPythonModule& pyModule, PyObject* (*initfunc)());
	void CreateLoggerInterface      (mvPythonModule& pyModule, PyObject* (*initfunc)());
	void CreatePlotInterface        (mvPythonModule& pyModule, PyObject* (*initfunc)());
	void CreateDrawingInterface     (mvPythonModule& pyModule, PyObject* (*initfunc)());
	void CreateWidgetAddingInterface(mvPythonModule& pyModule, PyObject* (*initfunc)());
	void CreateConstantsInterface   (mvPythonModule& pyModule, PyObject* (*initfunc)());

}