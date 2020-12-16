#pragma once

#include "mvAppItem.h"
#include "mvApp.h"
#include "mvAppLog.h"
#include "mvPythonParser.h"

namespace Marvel {

	PyObject* add_table(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvTable : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE(mvAppItemType::Table, "add_table")

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

		[[nodiscard]] std::string getTableItem (int row, int column) const;
		[[nodiscard]] PyObject*   getSelections() const;

		void                    setPyValue(PyObject* value);
		[[nodiscard]] PyObject* getPyValue() const;
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

	};

}
