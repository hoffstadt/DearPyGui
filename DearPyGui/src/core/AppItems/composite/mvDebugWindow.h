#pragma once

#include "mvTypeBases.h"
#include "mvModule_Core.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDebugWindow, MV_ITEM_DESC_ROOT, StorageValueTypes::None, 1);
	class mvDebugWindow : public mvBaseWindowAppitem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDebugWindow, add_debug_window)

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		mvDebugWindow(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void renderItem(mvAppItem* item);

	private:

		std::string m_selectedItem;
		ImGuiID     m_selectedID = 0u;
		std::vector<std::pair<std::string, std::string>> m_commands;
	};

}