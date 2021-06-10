#pragma once

#include "mvTypeBases.h"
#include <vector>

namespace Marvel {

	MV_REGISTER_WIDGET(mvTableNextColumn, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 1);
	class mvTableNextColumn : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTableNextColumn, add_table_next_column)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvTableNextColumn(mvUUID uuid);

		bool isParentCompatible(mvAppItemType type) override;

		void draw(ImDrawList* drawlist, float x, float y) override;

	};

}
