# pragma once

#include "Core/PythonUtilities/mvPythonTranslator.h"
#include <map>
#include <string>

namespace Marvel {

	class mvPythonModule;
	using pyInitFunc = mvPythonModule * (*)();
	using pyDocFunc = std::map<std::string, mvPythonTranslator>(*)();

	std::vector<std::pair<std::string, long>> BuildConstantsInterface();

	std::map<std::string, mvPythonTranslator> BuildAppInterface();
	std::map<std::string, mvPythonTranslator> BuildWidgetsInterface();
	std::map<std::string, mvPythonTranslator> BuildDrawingInterface();
	std::map<std::string, mvPythonTranslator> BuildInputsInterface();
	std::map<std::string, mvPythonTranslator> BuildLoggingInterface();
	std::map<std::string, mvPythonTranslator> BuildPlottingInterface();

	mvPythonModule* CreatePythonInterface      ();
	mvPythonModule* CreateInputInterface       ();
	mvPythonModule* CreateLoggerInterface      ();
	mvPythonModule* CreatePlotInterface        ();
	mvPythonModule* CreateDrawingInterface     ();
	mvPythonModule* CreateWidgetAddingInterface();
	mvPythonModule* CreateConstantsInterface   ();

	template<int index>
	constexpr void initModule(const char* name, pyInitFunc interfacefunc, pyDocFunc docfunc = nullptr)
	{
		mvInterfaceRegistry::GetRegistry()->addModule(name, interfacefunc, docfunc);

		auto initfunc = []() {
			mvPythonModule* pmod = mvInterfaceRegistry::GetRegistry()->getInitFunc(index)();
			PyObject* moddef = PyModule_Create(pmod->getModuleDefinition());
			for (auto& item : pmod->getIntConstants())
				PyModule_AddIntConstant(moddef, item.first.c_str(), item.second);
			return moddef;
		};

		PyImport_AppendInittab(name, initfunc);
	}

	constexpr void initModules()
	{
		initModule<0>("sbApp", CreatePythonInterface, BuildAppInterface);
		initModule<1>("sbInput", CreateInputInterface, BuildInputsInterface);
		initModule<2>("sbLog", CreateLoggerInterface, BuildLoggingInterface);
		initModule<3>("sbPlot", CreatePlotInterface, BuildPlottingInterface);
		initModule<4>("sbDraw", CreateDrawingInterface, BuildDrawingInterface);
		initModule<5>("sbWidgets", CreateWidgetAddingInterface, BuildWidgetsInterface);
		initModule<6>("sbConstants", CreateConstantsInterface);
	}

}