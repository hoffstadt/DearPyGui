#pragma once

#include "mvPythonParser.h"
#include "mvPythonModule.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// Forward Declarations
	//-----------------------------------------------------------------------------
	class mvPythonModule;
	class mvPythonParser;

	//-----------------------------------------------------------------------------
	// Typedefs
	//-----------------------------------------------------------------------------
	using pyInitFunc = mvPythonModule * (*)();
	using pyDocFunc = std::map<std::string, mvPythonParser>&(*)();

	//-----------------------------------------------------------------------------
	// mvInterfaceRegistry
	//-----------------------------------------------------------------------------
	class mvInterfaceRegistry
	{

	public:

		static mvInterfaceRegistry* GetRegistry();

		std::vector<std::pair<std::string, long>>& getConstants              ();
		std::vector<long>                          getConstantsValue         ();
		std::vector<const char*>                   getConstantsCommands      ();
		std::map<std::string, mvPythonParser>& getPythonInterface        (const std::string& name);
		std::vector<const char*>                   getPythonInterfaceCommands(const std::string& name);
		std::vector<const char*>                   getPythonInterfaceDoc     (const std::string& name);
		void                                       addModule                 (const char* name, pyInitFunc initfunc, pyDocFunc docfunc = nullptr);
		pyInitFunc                                 getInitFunc               (int i);
		
		std::vector<std::pair<std::string, std::string>> getAllCommands();

	private:

		mvInterfaceRegistry();

	private:

		static mvInterfaceRegistry*                                      s_instance;
		std::map<std::string, std::map<std::string, mvPythonParser>> m_translators;
		std::vector<std::pair<std::string, long>>                        m_constants;
		std::vector<pyInitFunc>                                          m_modules;

	};




}