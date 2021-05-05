#pragma once
#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include <stdint.h>

namespace Marvel {

	MV_REGISTER_WIDGET(mvTableRow, MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 2);
	class mvTableRow : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);


		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTableRow, add_table_row)

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		mvTableRow(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y) override;
		bool isParentCompatible(mvAppItemType type) override;

	private:

		int m_id = 0;
		bool m_draggable = true;
	};

}