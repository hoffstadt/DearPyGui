#pragma once

//-----------------------------------------------------------------------------
// mvInterfaceRegistry
//
//     - This class acts as a single location where python modules, parsers,
//       etc. are registered. They can then be queried by other classes
//       for documentation, initialization, etc.
//
//     - This class is confusing and will need to be reworked for v0.2
//     
//-----------------------------------------------------------------------------

#include "mvPythonParser.h"
#include "mvPythonModule.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// Forward Declarations
	//-----------------------------------------------------------------------------
	class mvPythonModule;
	class mvPythonParser;
	std::vector<std::pair<std::string, long>> BuildConstantsInterface();
	std::map<std::string, mvPythonParser>&    BuildMarvelInterface();
	mvPythonModule* CreateMarvelInterface();
	mvPythonModule* CreateConstantsInterface();

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

		void generateStubFile(const std::string& file);

		std::vector<std::pair<std::string, long>>& getConstants              ();
		std::vector<long>                          getConstantsValue         ();
		std::vector<const char*>                   getConstantsCommands      ();
		std::map<std::string, mvPythonParser>&     getPythonInterface        (const std::string& name);
		std::vector<const char*>                   getPythonInterfaceCommands(const std::string& name);
		std::vector<const char*>                   getPythonInterfaceDoc     (const std::string& name);
		void                                       addModule                 (const char* name, pyInitFunc initfunc, pyDocFunc docfunc = nullptr);
		pyInitFunc                                 getInitFunc               (int i);
		
		std::vector<std::pair<std::string, std::string>> getAllCommands();

	private:

		mvInterfaceRegistry();

	private:

		static mvInterfaceRegistry*                                      s_instance;
		std::map<std::string, std::map<std::string, mvPythonParser>>     m_parsers;
		std::vector<std::pair<std::string, long>>                        m_constants;
		std::vector<pyInitFunc>                                          m_modules;
	};




}