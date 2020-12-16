#pragma once

#include "mvTypeBases.h"
#include <vector>
#include "mvPythonParser.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvManagedColumns
//     * mvColumn
//     * mvNextColumn
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvManagedColumns
	//-----------------------------------------------------------------------------

	PyObject* add_managed_columns(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvManagedColumns : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE(mvAppItemType::ManagedColumns, "add_managed_columns")

		mvManagedColumns(const std::string& name, int columns);

		void  draw              ()               override;
		void  setExtraConfigDict(PyObject* dict) override;
		void  getExtraConfigDict(PyObject* dict) override;
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

	PyObject* add_columns(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvColumn : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE(mvAppItemType::ColumnSet, "add_columns")

		mvColumn(const std::string& name, int columns);

		void draw              ()               override;
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		int  m_columns = 1;
		bool m_border = true;

	};

	//-----------------------------------------------------------------------------
	// mvNextColumn
	//-----------------------------------------------------------------------------

	PyObject* add_next_column(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvNextColumn : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE(mvAppItemType::NextColumn, "add_next_column")

		mvNextColumn(const std::string& name);

		void draw() override;

	};

}
