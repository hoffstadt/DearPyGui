#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvSeparator, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 1);
	class mvSeparator : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION2(mvAppItemType::mvSeparator, add_separator)

		MV_CREATE_CONSTANT(mvThemeCol_Separator, ImGuiCol_Separator, 0L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Separator, mvColor(110, 110, 128, 128)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		mvSeparator(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y) override;

	};

}