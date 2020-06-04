#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <vector>
#include <memory>

#define addMethod(Function, Documentation) addMethod_(#Function, Function, Documentation)

#define MV_DECLARE_PYMODULE(x, name) mvPythonModule x(name);PyObject* PyInit_Emb##x(void){return PyModule_Create(x.getModuleDefinition());};

#define MV_INIT_PYMODULE(x) CreatePythonInterface(x, &PyInit_Emb##x);

namespace Marvel {

	class mvPythonModule
	{

	public:

		explicit mvPythonModule(const char* name);

		PyModuleDef* getModuleDefinition();

		void addMethod_(const char* name, PyCFunction function, const char* documentation = "No documentation");
		void addMethod_(const char* name, PyCFunctionWithKeywords function, const char* documentation = "No documentation");

		inline const char* getName() const { return m_name; }

	private:

		const char*                  m_name;
		std::vector<PyMethodDef>     m_methods;
		std::shared_ptr<PyModuleDef> m_moduledef = nullptr;

	};

}