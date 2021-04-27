#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "mvTypeBases.h"
#include "mvModule_Core.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDocWindow, MV_ITEM_DESC_ROOT, StorageValueTypes::None, 1);
	class mvDocWindow : public mvBaseWindowAppitem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDocWindow, add_doc_window)

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		mvDocWindow(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void setup();

	private:

		int categorySelection = 0;
		const char* m_doc = "None";

		std::vector<const char*> m_categories;

		std::unordered_map<std::string, std::vector<const char*>> m_commandCategories;
		std::unordered_map<std::string, std::vector<const char*>> m_docCategories;

		std::vector<const char*> m_cconstants;

		std::vector<std::pair<std::string, std::string>> m_commands;
		
		std::vector<std::string> m_constantsValues;

	};

}