#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvColorValue, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Color, 1);
	class mvColorValue : public mvColorPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvColorValue, add_color_value)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvColorValue(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override {}

	};

}
