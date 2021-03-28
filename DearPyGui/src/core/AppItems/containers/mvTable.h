#pragma once

#include "mvTypeBases.h"
#include <vector>

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvTable
//     * mvTableColumn
//     * mvTableHeaderRow
//     * mvTableNextColumn
//
//-----------------------------------------------------------------------------

namespace Marvel {

#ifdef MV_CPP
#else
	PyObject* add_table           (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_table_column    (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_table_header_row(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_table_next_column     (PyObject* self, PyObject* args, PyObject* kwargs);
#endif

	//-----------------------------------------------------------------------------
	// mvTableColumn
	//-----------------------------------------------------------------------------
	MV_REGISTER_WIDGET(mvTableColumn);
	class mvTableColumn : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE(mvAppItemType::mvTableColumn, "add_table_column")

			MV_START_COLOR_CONSTANTS
			MV_END_COLOR_CONSTANTS

			MV_START_STYLE_CONSTANTS
			MV_END_STYLE_CONSTANTS

		mvTableColumn(const std::string& name, float init_width_or_weight);
		~mvTableColumn();

		void draw() override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		ImGuiTableColumnFlags m_flags = 0;
		float m_init_width_or_weight = 0.0f;

	};

	//-----------------------------------------------------------------------------
	// mvTableHeaderRow
	//-----------------------------------------------------------------------------
	MV_REGISTER_WIDGET(mvTableHeaderRow);
	class mvTableHeaderRow : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE(mvAppItemType::mvTableHeaderRow, "add_table_header_row")

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

		mvTableHeaderRow(const std::string& name);

		void draw() override;

	};

	//-----------------------------------------------------------------------------
	// mvNextColumn
	//-----------------------------------------------------------------------------
	MV_REGISTER_WIDGET(mvTableNextColumn);
	class mvTableNextColumn : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE(mvAppItemType::mvTableNextColumn, "add_table_next_column")

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

		mvTableNextColumn(const std::string& name);

		void draw() override;

	};

	//-----------------------------------------------------------------------------
	// mvTable
	//-----------------------------------------------------------------------------
	MV_REGISTER_WIDGET(mvTable);
	class mvTable : public mvAppItem
	{
		MV_APPITEM_TYPE(mvAppItemType::mvTable, "add_table")

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvTable(const std::string& name, int inner_width);

		void draw() override;

		void incrementColumns();
		void decrementColumns();
		const std::string& getLastColumnAdded() const;
		void setLastColumnAdded(const std::string& name);
		void setRowHeader(const std::string& name);

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		int  m_columns = 0;
		int  m_inner_width = 0;
		ImGuiTableFlags m_flags = 0;
		
		// this keeps track of the last column so
		// addAfter() can use this to place
		// columns correctly.
		std::string m_lastColumnAdded;
		std::string m_rowHeader;
	};

}
