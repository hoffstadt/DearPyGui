#pragma once

#include "mvItemRegistry.h"
#include <vector>

namespace Marvel {

	MV_REGISTER_WIDGET(mvTableNextColumn, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 1);
	class mvTableNextColumn : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTableNextColumn, add_table_next_column)
		MV_NO_COMMANDS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

		MV_START_PARENTS
			MV_ADD_PARENT(mvAppItemType::mvStagingContainer),
			MV_ADD_PARENT(mvAppItemType::mvTable)
		MV_END_PARENTS

	public:

		mvTableNextColumn(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;

	};

}
