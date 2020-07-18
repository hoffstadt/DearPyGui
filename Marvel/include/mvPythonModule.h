#pragma once

//-----------------------------------------------------------------------------
// mvPythonModule
//
//     - This class acts as a wrapper over the Python C API module system.
//       It neatly hides away the ugly C details of the API.
//     
//-----------------------------------------------------------------------------

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <vector>
#include <memory>
#include <string>

//-----------------------------------------------------------------------------
// Helper Macro
//-----------------------------------------------------------------------------
#define addMethodD(Function) addMethod_(#Function, Function, Parsers[#Function].getDocumentation())

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvPythonModule
	//-----------------------------------------------------------------------------
	class mvPythonModule
	{

	public:

		mvPythonModule(const char* name, const std::vector<std::pair<std::string, long>>& constants);

		PyModuleDef*                                     getModuleDefinition();
		void                                             setIntConstants    (const std::vector<std::pair<std::string, long>>& constants) { m_intconstants = constants; }
		const std::vector<std::pair<std::string, long>>& getIntConstants    () const { return m_intconstants; }
		void                                             addMethod_         (const char* name, PyCFunction function, const char* documentation);
		void                                             addMethod_         (const char* name, PyCFunctionWithKeywords function, const char* documentation);
		inline const std::string&                        getName            () const { return m_name; }

	private:

		std::string                               m_name;
		std::vector<PyMethodDef>                  m_methods;
		std::shared_ptr<PyModuleDef>              m_moduledef = nullptr;
		std::vector<std::pair<std::string, long>> m_intconstants;

	};

}