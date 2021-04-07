#pragma once

#include "mvAppItem.h"
#include "mvApp.h"
#include "mvAppLog.h"
#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvDataGrid, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 1);
	class mvDataGrid : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDataGrid, add_data_grid)

		MV_CREATE_EXTRA_COMMAND(set_grid_headers);
		MV_CREATE_EXTRA_COMMAND(set_grid_data);
		MV_CREATE_EXTRA_COMMAND(get_grid_data);
		MV_CREATE_EXTRA_COMMAND(clear_data_grid);
		MV_CREATE_EXTRA_COMMAND(get_grid_item);
		MV_CREATE_EXTRA_COMMAND(set_grid_item);
		MV_CREATE_EXTRA_COMMAND(get_grid_selections);
		MV_CREATE_EXTRA_COMMAND(set_grid_selection);
		MV_CREATE_EXTRA_COMMAND(add_grid_column);
		MV_CREATE_EXTRA_COMMAND(insert_grid_column);
		MV_CREATE_EXTRA_COMMAND(delete_grid_column);
		MV_CREATE_EXTRA_COMMAND(add_grid_row);
		MV_CREATE_EXTRA_COMMAND(insert_grid_row);
		MV_CREATE_EXTRA_COMMAND(delete_grid_row);

		MV_CREATE_CONSTANT(mvThemeCol_DataGrid_Text					, ImGuiCol_Text						, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_DataGrid_HeaderBg				, ImGuiCol_TableHeaderBg			, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_DataGrid_RowBg				, ImGuiCol_TableRowBg				, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_DataGrid_RowBgAlt				, ImGuiCol_TableRowBgAlt			, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_DataGrid_Item					, ImGuiCol_Header					, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_DataGrid_ItemHovered			, ImGuiCol_HeaderHovered			, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_DataGrid_ItemActive			, ImGuiCol_HeaderActive				, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_DataGrid_BorderStrong			, ImGuiCol_TableBorderStrong		, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_DataGrid_BorderLight			, ImGuiCol_TableBorderLight			, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_DataGrid_BorderHovered		, ImGuiCol_SeparatorHovered			, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_DataGrid_BorderActive			, ImGuiCol_SeparatorActive			, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_DataGrid_CellPaddingX		, ImGuiStyleVar_CellPadding			, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_DataGrid_CellPaddingY		, ImGuiStyleVar_CellPadding			, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_DataGrid_ItemSpacingX		, ImGuiStyleVar_ItemSpacing			, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_DataGrid_ItemSpacingY		, ImGuiStyleVar_ItemSpacing			, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_DataGrid_ItemTextAlignX		, ImGuiStyleVar_SelectableTextAlign	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_DataGrid_ItemTextAlignY		, ImGuiStyleVar_SelectableTextAlign	, 1L);

		MV_START_EXTRA_COMMANDS
			MV_ADD_EXTRA_COMMAND(set_grid_headers);
			MV_ADD_EXTRA_COMMAND(set_grid_data);
			MV_ADD_EXTRA_COMMAND(get_grid_data);
			MV_ADD_EXTRA_COMMAND(clear_data_grid);
			MV_ADD_EXTRA_COMMAND(get_grid_item);
			MV_ADD_EXTRA_COMMAND(set_grid_item);
			MV_ADD_EXTRA_COMMAND(get_grid_selections);
			MV_ADD_EXTRA_COMMAND(set_grid_selection);
			MV_ADD_EXTRA_COMMAND(add_grid_column);
			MV_ADD_EXTRA_COMMAND(insert_grid_column);
			MV_ADD_EXTRA_COMMAND(delete_grid_column);
			MV_ADD_EXTRA_COMMAND(add_grid_row);
			MV_ADD_EXTRA_COMMAND(insert_grid_row);
			MV_ADD_EXTRA_COMMAND(delete_grid_row);
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DataGrid_Text,                 mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DataGrid_HeaderBg,			  mvColor( 48,  48,  51, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DataGrid_RowBg,				  mvColor(  0,   0,   0,   0)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DataGrid_RowBgAlt,			  mvColor(255, 255, 255,  15)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DataGrid_Item,                 mvColor( 66, 150, 250,  79)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DataGrid_ItemHovered,          mvColor( 66, 150, 250, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DataGrid_ItemActive,           mvColor( 15, 135, 250, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DataGrid_BorderStrong,		  mvColor( 79,  79,  89, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DataGrid_BorderLight,		  mvColor( 59,  59,  64, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DataGrid_BorderHovered,        mvColor( 26, 102, 191, 199)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DataGrid_BorderActive,         mvColor( 26, 102, 191, 255)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT(mvThemeStyle_DataGrid_CellPaddingX			, 4, 20),
			MV_ADD_CONSTANT(mvThemeStyle_DataGrid_CellPaddingY			, 2, 20),
			MV_ADD_CONSTANT(mvThemeStyle_DataGrid_ItemSpacingX			, 8, 20),
			MV_ADD_CONSTANT(mvThemeStyle_DataGrid_ItemSpacingY			, 4, 20),
			MV_ADD_CONSTANT(mvThemeStyle_DataGrid_ItemTextAlignX		, 0,  1),
			MV_ADD_CONSTANT(mvThemeStyle_DataGrid_ItemTextAlignY		, 0,  1),
		MV_END_STYLE_CONSTANTS

	public:

		mvDataGrid(const std::string& name);

		// table operations
		void setGridItem   (int row, int column, const std::string& value);
		void setSelection  (int row, int column, bool value);
		void addHeaders    (const std::vector<std::string>& headers);
		void addRow        (const std::vector<std::string>& row);
		void insertRow     (int row_index, const std::vector<std::string>& row);
		void deleteRow     (int row);
		void addColumn     (const std::string& name, const std::vector<std::string>& column);
		void insertColumn  (int column_index, const std::string& name, const std::vector<std::string>& column);
		void deleteColumn  (int column);
		void clearGrid     ();
		int  getColumnCount() const { return (int)m_columns; }
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

		[[nodiscard]] std::string getGridItem (int row, int column) const;
		[[nodiscard]] PyObject*   getSelections() const;

		void                    setPyValue(PyObject* value) override;
		[[nodiscard]] PyObject* getPyValue() override;
		void draw(ImDrawList* drawlist, float x, float y) override;

	private:

		[[nodiscard]] bool isIndexValid(int row, int column) const;
		void               updateHashValues();

	private:

		std::map<std::pair<int, int>, bool>   m_selections;
		std::vector<std::string>              m_headers;
		std::vector<std::vector<std::string>> m_hashValues;
		std::vector<std::vector<std::string>> m_values;
		size_t                                m_columns;
		bool                                  m_hide_headers;

		// new table api items
		ImGuiTableFlags m_flags = 0;

	};

}
