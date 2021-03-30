#pragma once

#include "mvTypeBases.h"
#include <vector>

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvTable
//     * mvTableColumn
//     * mvTableNextColumn
//
//-----------------------------------------------------------------------------

namespace Marvel {

#ifdef MV_CPP
#else
	PyObject* add_table           (PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_table_column    (PyObject* self, PyObject* args, PyObject* kwargs);
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
		bool isParentCompatible(mvAppItemType type) override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		ImGuiTableColumnFlags m_flags = 0;
		float m_init_width_or_weight = 0.0f;

	};

	//-----------------------------------------------------------------------------
	// mvTableNextColumn
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

		bool isParentCompatible(mvAppItemType type) override;

		void draw() override;

	};

	//-----------------------------------------------------------------------------
	// mvTable
	//-----------------------------------------------------------------------------
	MV_REGISTER_WIDGET(mvTable);
	class mvTable : public mvAppItem
	{
		MV_APPITEM_TYPE(mvAppItemType::mvTable, "add_table")

		MV_CREATE_THEME_CONSTANT(mvThemeCol_TableHeaderBg,		44L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_TableBorderStrong,	45L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_TableBorderLight,	46L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_TableRowBg,			47L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_TableRowBgAlt,		48L, 0L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TableHeaderBg,		mvColor( 48,  48,  51, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TableBorderStrong,	mvColor( 79,  79,  89, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TableBorderLight,	mvColor( 59,  59,  64, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TableRowBg,			mvColor(  0,   0,   0,   0)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TableRowBgAlt,		mvColor(255, 255, 255,  15))
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

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		int  m_columns = 0;
		int  m_inner_width = 0;
		ImGuiTableFlags m_flags = 0;
		bool m_tableHeader = true;
		
		// this keeps track of the last column so
		// addAfter() can use this to place
		// columns correctly.
		std::string m_lastColumnAdded;
	};

}
