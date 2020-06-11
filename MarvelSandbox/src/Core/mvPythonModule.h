#pragma once

//-----------------------------------------------------------------------------
// mvPythonModule
//
//     - This class (and its macro helpers) act as a light wrapper on the 
//       python extension process, automating the following:
//
//           ~ PyMethodDef array construction
//           ~ PyModuleDef struct construction
//           ~ Casting to PyCfunction
//           ~ Calling PyModule_Create
//
//     - This class should ONLY be used through the helper macros below.
//
//     - How to Use:
//           ~ Create a function with the following signature:
//                 void(mvPythonModule& pyModule, PyObject* (*initfunc)())
//
//           ~ Inside the function, begin adding "methods" to the pyModule 
//             using the addMethod(x, y) macro where x is the method to add
//             and y is a documentation string
//
//           ~ After all the functions are added, run:
//              PyImport_AppendInittab(pyModule.getName(), initfunc);
//
//           ~ Before main (globally), call MV_DECLARE_PYMODULE(x, y), where
//             x is an identifier later used by MV_INIT_PYMODULE and y is
//             a string that the module be referred to from python.
//
//           ~ Declare the functions created in step 1.
//
//           ~ Before Py_Initialize(), call MV_INIT_PYMODULE(x, y) where x
//             is the identifier used in MV_DECLARE_PYMODULE and y is the
//             function decalred in step 1.
//
//-----------------------------------------------------------------------------

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <vector>
#include <memory>
#include <string>

//-----------------------------------------------------------------------------
// Helper Macros
//-----------------------------------------------------------------------------
#define addMethod(Function, Documentation) addMethod_(#Function, Function, Documentation)

#define MV_DECLARE_PYMODULE(x, name, constants) mvPythonModule x(name, constants);\
PyObject* PyInit_Emb##x(void){\
PyObject* mod = PyModule_Create(x.getModuleDefinition());\
for(auto& item : x.getIntConstants())\
PyModule_AddIntConstant(mod, item.first.c_str(), item.second);\
return mod;};

#define MV_INIT_PYMODULE(x, y) y(x, &PyInit_Emb##x);

namespace Marvel {

	class mvPythonModule
	{

	public:

		mvPythonModule(const char* name, const std::vector<std::pair<std::string, long>>& constants);

		PyModuleDef*                                     getModuleDefinition();
		void                                             setIntConstants(const std::vector<std::pair<std::string, long>>& constants) { m_intconstants = constants; }
		const std::vector<std::pair<std::string, long>>& getIntConstants() const { return m_intconstants; }
		void                                             addMethod_(const char* name, PyCFunction function, const char* documentation = "No documentation");
		void                                             addMethod_(const char* name, PyCFunctionWithKeywords function, const char* documentation = "No documentation");
		inline const char*                               getName() const { return m_name; }

	private:

		const char*                  m_name;
		std::vector<PyMethodDef>     m_methods;
		std::shared_ptr<PyModuleDef> m_moduledef = nullptr;
		std::vector<std::pair<std::string, long>>  m_intconstants;

	};

}