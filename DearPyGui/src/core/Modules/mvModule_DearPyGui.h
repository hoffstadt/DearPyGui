#pragma once

#include <map>
#include <vector>
#include "mvPythonParser.h"

#define PY_SSIZE_T_CLEAN
#include <Python.h>

//-----------------------------------------------------------------------------
// Helper Macro
//-----------------------------------------------------------------------------
#define ADD_PYTHON_FUNCTION(Function) methods.push_back({ #Function, (PyCFunction)Function, METH_VARARGS | METH_KEYWORDS, GetParsers()[#Function].getDocumentation() })

namespace Marvel {

	PyMODINIT_FUNC PyInit__dearpygui(void);

	const std::map<std::string, mvPythonParser>&     GetModuleParsers();
	const std::vector<std::pair<std::string, long>>& GetModuleConstants();

}