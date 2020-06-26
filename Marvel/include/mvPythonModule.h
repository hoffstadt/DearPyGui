#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <vector>
#include <memory>
#include <string>

//-----------------------------------------------------------------------------
// Helper Macros
//-----------------------------------------------------------------------------
#define addMethod(Function, Documentation) addMethod_(#Function, Function, Documentation)
#define addMethodD(Function) addMethod_(#Function, Function, Translators[#Function].getDocumentation())

namespace Marvel {

	class mvPythonModule
	{

	public:

		mvPythonModule(const char* name, const std::vector<std::pair<std::string, long>>& constants);

		PyModuleDef*                                     getModuleDefinition();
		void                                             setIntConstants    (const std::vector<std::pair<std::string, long>>& constants) { m_intconstants = constants; }
		const std::vector<std::pair<std::string, long>>& getIntConstants    () const { return m_intconstants; }
		void                                             addMethod_         (const char* name, PyCFunction function, const char* documentation = "No documentation");
		void                                             addMethod_         (const char* name, PyCFunctionWithKeywords function, const char* documentation = "No documentation");
		inline const std::string&                        getName            () const { return m_name; }

	private:

		std::string                                m_name;
		std::vector<PyMethodDef>                   m_methods;
		std::shared_ptr<PyModuleDef>               m_moduledef = nullptr;
		std::vector<std::pair<std::string, long>>  m_intconstants;

	};

}