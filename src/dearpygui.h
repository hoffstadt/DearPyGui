#pragma once

#include <map>
#include <vector>
#include "mvPyUtils.h"

PyMODINIT_FUNC PyInit__dearpygui(void);

const std::map<std::string, mvPythonParser>& GetModuleParsers();
const std::vector<std::pair<std::string, long>>& GetModuleConstants();
const std::vector<std::pair<std::string, long>>& GetKeysConstants();
