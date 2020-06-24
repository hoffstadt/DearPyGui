#pragma once

#include "Core/PythonUtilities/mvPythonTranslator.h"

namespace Marvel {

	class mvInterfaceRegistry
	{

	public:

		static mvInterfaceRegistry* GetRegistry();

		std::vector<std::pair<std::string, long>>&       getConstants() { return m_constants; }
		std::vector<long>                                getConstantsValue();
		std::vector<const char*>                         getConstantsCommands();
		std::map<std::string, mvPythonTranslator>&       getPythonInterface(const std::string& name) { return m_translators[name]; }
		std::vector<const char*>                         getPythonInterfaceCommands(const std::string& name);
		std::vector<const char*>                         getPythonInterfaceDoc(const std::string& name);

	private:

		mvInterfaceRegistry();

	private:

		static mvInterfaceRegistry* s_instance;
		std::map<std::string, std::map<std::string, mvPythonTranslator>> m_translators;
		std::vector<std::pair<std::string, long>> m_constants;

	};

}