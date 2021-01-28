#pragma once

//-----------------------------------------------------------------------------
// mvMarvel
//
//     - This file contains marvel python module command information.
//
//     - These parsers are used to check python parsing and build documentation
//     
//-----------------------------------------------------------------------------

#include "mvPython.h"
#include <map>

namespace Marvel {

	PyMODINIT_FUNC PyInit_core(void);

	mvRef<std::map<std::string, mvPythonParser>> BuildDearPyGuiInterface();

	std::vector<std::pair<std::string, std::string>> GetAllCommands();

	std::vector<std::pair<std::string, long>> GetModuleConstants();

	void start_dearpygui_error();
}
