#pragma once

#include "mvItemRegistry.h"
#include <vector>

namespace Marvel {

	MV_REGISTER_WIDGET(mvTable, MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
	class mvTable : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTable, add_table)
		MV_DEFAULT_PARENTS
		MV_DEFAULT_CHILDREN
		
		MV_CREATE_COMMAND(highlight_table_column);
		MV_CREATE_COMMAND(unhighlight_table_column);
		MV_CREATE_COMMAND(set_table_row_color);
		MV_CREATE_COMMAND(unset_table_row_color);
		MV_CREATE_COMMAND(highlight_table_cell);
		MV_CREATE_COMMAND(unhighlight_table_cell);
		MV_CREATE_COMMAND(highlight_table_row);
		MV_CREATE_COMMAND(unhighlight_table_row);
		MV_CREATE_COMMAND(is_table_column_highlighted);
		MV_CREATE_COMMAND(is_table_row_highlighted);
		MV_CREATE_COMMAND(is_table_cell_highlighted);

		MV_CREATE_CONSTANT(mvTable_SizingFixedFit, ImGuiTableFlags_SizingFixedFit);
		MV_CREATE_CONSTANT(mvTable_SizingFixedSame, ImGuiTableFlags_SizingFixedSame);
		MV_CREATE_CONSTANT(mvTable_SizingStretchProp, ImGuiTableFlags_SizingStretchProp);
		MV_CREATE_CONSTANT(mvTable_SizingStretchSame, ImGuiTableFlags_SizingStretchSame);

		MV_START_COMMANDS
			MV_ADD_COMMAND(highlight_table_column);
			MV_ADD_COMMAND(unhighlight_table_column);
			MV_ADD_COMMAND(set_table_row_color);
			MV_ADD_COMMAND(unset_table_row_color);
			MV_ADD_COMMAND(highlight_table_cell);
			MV_ADD_COMMAND(unhighlight_table_cell);
			MV_ADD_COMMAND(highlight_table_row);
			MV_ADD_COMMAND(unhighlight_table_row);
			MV_ADD_COMMAND(is_table_column_highlighted);
			MV_ADD_COMMAND(is_table_row_highlighted);
			MV_ADD_COMMAND(is_table_cell_highlighted);
		MV_END_COMMANDS

		MV_START_CONSTANTS
			MV_ADD_CONSTANT(mvTable_SizingFixedFit),
			MV_ADD_CONSTANT(mvTable_SizingFixedSame),
			MV_ADD_CONSTANT(mvTable_SizingStretchProp),
			MV_ADD_CONSTANT(mvTable_SizingStretchSame)
		MV_END_CONSTANTS

	public:

		mvTable(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;
		PyObject* getPyValue() override;
		void setPyValue(PyObject* value) override;
		void onChildAdd(mvRef<mvAppItem> item) override;
		void onChildRemoved(mvRef<mvAppItem> item) override;
		void onChildrenRemoved() override;
		void applySpecificTemplate(mvAppItem* item) override;

	private:

		ImGuiTextFilter _imguiFilter;

		int  _columns = 0;
		int  _rows = 0;
		int  _inner_width = 0;
		int  _freezeRows = 0;
		int  _freezeColumns = 0;
		ImGuiTableFlags _flags = 0;
		bool _tableHeader = true;
		bool _useClipper = false;

		std::vector<bool> _columnColorsSet;
		std::vector<bool> _rowColorsSet;
		std::vector<bool> _rowSelectionColorsSet;
		std::vector<std::vector<bool>> _cellColorsSet;

		std::vector<ImU32> _columnColors;
		std::vector<ImU32> _rowColors;
		std::vector<ImU32> _rowSelectionColors;
		std::vector<std::vector<ImU32>> _cellColors;

		struct SortSpec 
		{
			mvUUID column;
			int direction;
		};
		
	};

}
