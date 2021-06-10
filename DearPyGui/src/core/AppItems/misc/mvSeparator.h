#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvSeparator, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 1);
	class mvSeparator : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvSeparator, add_separator)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvSeparator(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;

	};

}