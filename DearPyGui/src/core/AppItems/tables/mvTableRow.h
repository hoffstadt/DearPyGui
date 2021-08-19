#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include <stdint.h>

namespace Marvel {

	MV_REGISTER_WIDGET(mvTableRow, MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
	class mvTableRow : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);


		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTableRow, add_table_row)
		MV_NO_COMMANDS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

		MV_START_PARENTS
			MV_ADD_PARENT(mvAppItemType::mvStage),
			MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
			MV_ADD_PARENT(mvAppItemType::mvTable)
		MV_END_PARENTS

	public:

		explicit mvTableRow(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override {}

	private:

		int _id = 0;
	};

}