#pragma once

#include "mvTypeBases.h"
#include <vector>

namespace Marvel {

	MV_REGISTER_WIDGET(mvTableColumn, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 0);
	class mvTableColumn : public mvAppItem
	{
		
		// for access to ID
		friend class mvTable;

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTableColumn, add_table_column)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

		mvTableColumn(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		bool isParentCompatible(mvAppItemType type) override;

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		ImGuiTableColumnFlags m_flags = 0;
		float m_init_width_or_weight = 0.0f;
		ImGuiID m_id = 0u;

	};

}
