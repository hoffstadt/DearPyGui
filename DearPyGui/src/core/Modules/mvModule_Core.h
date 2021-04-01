#pragma once
#include "mvModule.h"

//-----------------------------------------------------------------------------
// Helper Macro
//-----------------------------------------------------------------------------
#define ADD_PYTHON_FUNCTION(Function) methods.push_back({ #Function, (PyCFunction)Function, METH_VARARGS | METH_KEYWORDS, mvApp::GetApp()->getParsers()[#Function].getDocumentation() })

namespace Marvel {

	PyMODINIT_FUNC PyInit_core(void);

	struct mvModule_Core : mvModule<mvModule_Core>
	{
		static const std::map<std::string, mvPythonParser>& GetSubModuleParsers();
		static const std::vector<std::pair<std::string, long>>& GetSubModuleConstants();
		static PyMethodDef* GetSubMethods();

	};


}