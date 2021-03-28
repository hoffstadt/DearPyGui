#pragma once

#include "mvAppItem.h"
#include "mvApp.h"
#include "mvAppLog.h"

namespace Marvel {

	struct mvDataGridConfig : public mvAppItemConfig
	{
		std::vector<std::string> headers;

		// features
		bool resizable               = false; // Enable resizing columns.
		bool reorderable             = false; // Enable reordering columns in header row (need calling TableSetupColumn() + TableHeadersRow() to display headers)
		bool hideable                = false; // Enable hiding/disabling columns in context menu.
		bool sortable                = false; // Enable sorting. 
		bool context_menu_in_body    = false; // Right-click on columns body/contents will display table context menu.
								     
		// decorations			     
		bool row_bg                  = false;
		bool borders_inner_h         = false; // Draw horizontal borders between rows.
		bool borders_outer_h         = false; // Draw horizontal borders at the top and bottom.
		bool borders_inner_v         = false; // Draw vertical borders between columns.
		bool borders_outer_v         = false; // Draw vertical borders on the left and right sides.

		// sizing policy
		
		// sizing options
		bool no_host_extend_x        = false; // Make outer width auto-fit to columns, overriding outer_size.x value. Only available when ScrollX/ScrollY are disabled and Stretch columns are not used.
		bool no_host_extend_y        = false; // Make outer height stop exactly at outer_size.y (prevent auto-extending table past the limit). Only available when ScrollX/ScrollY are disabled. Data below the limit will be clipped and not visible.
		bool no_keep_columns_visible = false; // Disable keeping column always minimally visible when ScrollX is off and table gets too small. Not recommended if columns are resizable.
		bool precise_widths          = false; // Disable distributing remainder width to stretched columns (width allocation on a 100-wide table with 3 columns: Without this flag: 33,33,34. With this flag: 33,33,33). With larger number of columns, resizing will appear to be less smooth.

		// clipping
		bool no_clip                 = false; // Disable clipping rectangle for every individual columns (reduce draw command count, items will be able to overflow into other columns). Generally incompatible with TableSetupScrollFreeze().
							         
		// padding			         
		bool pad_outer_x             = false; // Default if BordersOuterV is on. Enable outer-most padding. Generally desirable if you have headers.
		bool no_pad_outer_x          = false; // Default if BordersOuterV is off. Disable outer-most padding.
		bool no_pad_inner_x          = false; // Disable inner padding between columns (double inner padding if BordersOuterV is on, single inner padding if BordersOuterV is off).

		// scrolling
		bool scroll_x                = false; // Enable horizontal scrolling. Require 'outer_size' parameter of BeginTable() to specify the container siz
		bool scroll_y                = false; // Enable vertical scrolling. Require 'outer_size' parameter of BeginTable() to specify the container size.

	};

#ifdef MV_CPP
#else
	PyObject* add_data_grid(PyObject* self, PyObject* args, PyObject* kwargs);

	// data grid
	PyObject* set_grid_headers   (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_grid_data      (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_grid_data      (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* clear_data_grid    (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_grid_item      (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_grid_item      (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_grid_selections(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_grid_selection (PyObject* self, PyObject* args, PyObject* kwargs);

	// column
	PyObject* add_grid_column     (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* insert_grid_column  (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* delete_grid_column  (PyObject* self, PyObject* args, PyObject* kwargs);

	// row
	PyObject* add_grid_row        (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* insert_grid_row     (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* delete_grid_row     (PyObject* self, PyObject* args, PyObject* kwargs);

#endif // MV_CPP

	
	MV_REGISTER_WIDGET(mvDataGrid);
	class mvDataGrid : public mvAppItem
	{
		MV_APPITEM_TYPE(mvAppItemType::mvDataGrid, "add_data_grid")

		MV_CREATE_THEME_CONSTANT(mvThemeCol_DataGrid_Text					, 14L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_DataGrid_Cell					, 24L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_DataGrid_CellHovered			, 25L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_DataGrid_CellActive			    , 26L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_DataGrid_Border				    , 27L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_DataGrid_BorderHovered			, 28L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_DataGrid_BorderActive			, 29L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_DataGrid_Scrollbar				, 14L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_DataGrid_ScrollbarGrab			, 15L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_DataGrid_ScrollbarGrabHovered	, 16L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_DataGrid_ScrollbarGrabActive	, 17L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_DataGrid_ItemSpacingX			, 13L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_DataGrid_ItemSpacingY			, 13L, 1L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_DataGrid_SelectableTextAlignX	, 23L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_DataGrid_SelectableTextAlignY	, 23L, 1L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DataGrid_Text,                 mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DataGrid_Cell,                 mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DataGrid_CellHovered,          mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DataGrid_CellActive,           mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DataGrid_Border,               mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DataGrid_BorderHovered,        mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DataGrid_BorderActive,         mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DataGrid_Scrollbar,            mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DataGrid_ScrollbarGrab,        mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DataGrid_ScrollbarGrabHovered, mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_DataGrid_ScrollbarGrabActive,  mvColor(255, 255, 255, 255)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DataGrid_ItemSpacingX		, 4, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DataGrid_ItemSpacingY		, 4, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DataGrid_SelectableTextAlignX, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_DataGrid_SelectableTextAlignY, 0, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvDataGrid(const std::string& name, const std::vector<std::string>& headers);

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
		void                    draw      () override;

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
