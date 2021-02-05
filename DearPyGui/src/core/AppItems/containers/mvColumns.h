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

	PyObject* add_managed_columns(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_columns        (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_next_column    (PyObject* self, PyObject* args, PyObject* kwargs);

	//-----------------------------------------------------------------------------
	// mvManagedColumns
	//-----------------------------------------------------------------------------
	class mvManagedColumns : public mvAppItem
	{
		MV_APPITEM_TYPE(mvAppItemType::ManagedColumns, mvManagedColumns, "add_managed_columns")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::ManagedColumns, mvThemeCol_ManagedColumns_Border		, 27L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ManagedColumns, mvThemeCol_ManagedColumns_BorderHovered	, 28L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ManagedColumns, mvThemeCol_ManagedColumns_BorderActive	, 29L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ManagedColumns_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ManagedColumns_BorderHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ManagedColumns_BorderActive),
		MV_END_COLOR_CONSTANTS

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
	class mvColumn : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE_OLD_SYSTEM(mvAppItemType::ColumnSet, "add_columns")

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
	class mvNextColumn : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE_OLD_SYSTEM(mvAppItemType::NextColumn, "add_next_column")

		mvNextColumn(const std::string& name);

		void draw() override;

	};

}
