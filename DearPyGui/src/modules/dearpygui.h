#pragma once

#include <map>
#include <vector>
#include "mvPythonParser.h"

#define PY_SSIZE_T_CLEAN
#include <Python.h>

PyMODINIT_FUNC PyInit__dearpygui(void);

const std::map<std::string, mvPythonParser>& GetModuleParsers();
const std::vector<std::pair<std::string, long>>& GetModuleConstants();