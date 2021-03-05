#pragma once

#include "mvTypeBases.h"
#include <vector>

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvManagedColumns
//     * mvColumn
//     * mvNextColumn
//
//-----------------------------------------------------------------------------

namespace Marvel {

	struct mvManagedColumnsConfig : public mvAppItemConfig
	{
		int columns = 1;
		bool border = true;
	};

	PyObject* add_managed_columns(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_columns        (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_next_column    (PyObject* self, PyObject* args, PyObject* kwargs);

	//-----------------------------------------------------------------------------
	// mvManagedColumns
	//-----------------------------------------------------------------------------
	class mvManagedColumns : public mvAppItem
	{
		MV_APPITEM_TYPE(mvAppItemType::ManagedColumns, mvManagedColumns, "add_managed_columns")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::ManagedColumns, mvThemeCol_ManagedColumns_Border		, 27L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ManagedColumns, mvThemeCol_ManagedColumns_BorderHovered	, 28L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ManagedColumns, mvThemeCol_ManagedColumns_BorderActive	, 29L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ManagedColumns, mvThemeStyle_ManagedColumns_ItemSpacingX, 13L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ManagedColumns, mvThemeStyle_ManagedColumns_ItemSpacingY, 13L, 1L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ManagedColumns_Border,			mvColor(110, 110, 128, 128)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ManagedColumns_BorderHovered,	mvColor( 26, 102, 191, 199)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ManagedColumns_BorderActive,		mvColor( 26, 102, 191, 255)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ManagedColumns_ItemSpacingX, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ManagedColumns_ItemSpacingY, 0, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvManagedColumns(const std::string& name, int columns);

		void  draw              ()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

		void  setColumnWidth    (int i, float width);
		float getColumnWidth    (int i);

	private:

		int                m_previousColCount = 1;
		int                m_columns = 1;
		bool               m_border = true;
		std::vector<float> m_widths;
		bool               m_dirty_widths = false;
		bool               m_firstFrame = true; // only necessary for column widths

	};

	//-----------------------------------------------------------------------------
	// mvColumn
	//-----------------------------------------------------------------------------
	struct mvColumnConfig : public mvManagedColumnsConfig {};
	
	class mvColumn : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE(mvAppItemType::ColumnSet, mvColumn, "add_columns")

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

		mvColumn(const std::string& name, int columns);

		void draw              ()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		int  m_columns = 1;
		bool m_border = true;

	};

	//-----------------------------------------------------------------------------
	// mvNextColumn
	//-----------------------------------------------------------------------------
	struct mvNextColumnConfig : public mvAppItemConfig
	{
		mvNextColumnConfig()
		{
			name = "next_column";
		}
	};
	
	class mvNextColumn : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE(mvAppItemType::NextColumn, mvNextColumn, "add_next_column")

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

		mvNextColumn(const std::string& name);

		void draw() override;

	};

}
