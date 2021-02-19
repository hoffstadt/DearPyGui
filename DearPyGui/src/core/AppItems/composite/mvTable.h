#pragma once

#include "mvAppItem.h"
#include "mvApp.h"
#include "mvAppLog.h"

namespace Marvel {

	struct mvTableConfig : public mvAppItemConfig
	{
		std::vector<std::string> headers;

		mvTableConfig()
		{
			height = 200;
		}
	};

	PyObject* add_table(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvTable : public mvAppItem
	{
		MV_APPITEM_TYPE(mvAppItemType::Table, mvTable, "add_table")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::Table, mvThemeCol_Table_Text					, 14L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Table, mvThemeCol_Table_Cell					, 24L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Table, mvThemeCol_Table_CellHovered				, 25L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Table, mvThemeCol_Table_CellActive				, 26L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Table, mvThemeCol_Table_Border					, 27L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Table, mvThemeCol_Table_BorderHovered			, 28L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Table, mvThemeCol_Table_BorderActive			, 29L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Table, mvThemeCol_Table_Scrollbar				, 14L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Table, mvThemeCol_Table_ScrollbarGrab			, 15L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Table, mvThemeCol_Table_ScrollbarGrabHovered	, 16L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Table, mvThemeCol_Table_ScrollbarGrabActive		, 17L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Table, mvThemeStyle_Table_ItemSpacingX			, 13L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Table, mvThemeStyle_Table_ItemSpacingY			, 13L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Table, mvThemeStyle_Table_SelectableTextAlignX	, 23L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Table, mvThemeStyle_Table_SelectableTextAlignY	, 23L, 1L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Table_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Table_Cell),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Table_CellHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Table_CellActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Table_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Table_BorderHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Table_BorderActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Table_Scrollbar),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Table_ScrollbarGrab),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Table_ScrollbarGrabHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Table_ScrollbarGrabActive),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Table_ItemSpacingX		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Table_ItemSpacingY		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Table_SelectableTextAlignX, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Table_SelectableTextAlignY, 0, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvTable(const std::string& name, const std::vector<std::string>& headers);

		// table operations
		void setTableItem  (int row, int column, const std::string& value);
		void setSelection  (int row, int column, bool value);
		void addHeaders    (const std::vector<std::string>& headers);
		void addRow        (const std::vector<std::string>& row);
		void insertRow     (int row_index, const std::vector<std::string>& row);
		void deleteRow     (int row);
		void addColumn     (const std::string& name, const std::vector<std::string>& column);
		void insertColumn  (int column_index, const std::string& name, const std::vector<std::string>& column);
		void deleteColumn  (int column);
		void clearTable    ();
		int  getColumnCount() const { return (int)m_columns; }
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

		[[nodiscard]] std::string getTableItem (int row, int column) const;
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

	};

}
