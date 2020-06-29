#pragma once

#include <map>
#include "mvInterfaceRegistry.h"
#include "mvPythonModule.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// Forward Declarations
	//-----------------------------------------------------------------------------
	class mvPythonTranslator;

	//-----------------------------------------------------------------------------
	// Typedefs
	//-----------------------------------------------------------------------------
	using pyInitFunc = mvPythonModule * (*)();
	using pyDocFunc = std::map<std::string, mvPythonTranslator>&(*)();

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

	//-----------------------------------------------------------------------------
	// mvModuleInitializer
	//-----------------------------------------------------------------------------
	class mvModuleInitializer
	{

	public:

		static mvModuleInitializer* getInitializer();
		virtual void                initializeUserModules(int startindex) {}
		int                         initializeCoreModules();

	};

}