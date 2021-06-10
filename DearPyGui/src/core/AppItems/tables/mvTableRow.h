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

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvTableRow(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		bool isParentCompatible(mvAppItemType type) override;

	private:

		int m_id = 0;
	};

}