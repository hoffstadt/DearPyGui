#pragma once

#include <vector>
#include "mvApp.h"
#include "cpp.hint"
#include "mvToolWindow.h"

namespace Marvel{

	class mvFontManager;

	class mvToolManager
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);
		static void InsertConstants(std::vector<std::pair<std::string, long>>& constants);

		MV_CREATE_COMMAND(show_tool);

		MV_START_COMMANDS
			MV_ADD_COMMAND(show_tool);
		MV_END_COMMANDS

	public:

		static void Draw();

		static void ShowTool(mvUUID name);

		static mvFontManager& GetFontManager();


	private:

		mvToolManager();

		static std::vector<mvRef<mvToolWindow>> s_tools;

	};

}