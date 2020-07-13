#pragma once

#include "Core/AppItems/mvAppItem.h"
#include "Core/mvApp.h"
#include "Core/StandardWindows/mvAppLog.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvTable
//
//-----------------------------------------------------------------------------

namespace Marvel {

	class mvTable : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Table)

		mvTable(const std::string& parent, const std::string& name, const std::vector<std::string>& headers);

		// table operations
		void setTableItem(int row, int column, const std::string& value);
		void setSelection(int row, int column, bool value);
		void addRow      (const std::vector<std::string>& row);
		void insertRow   (int row_index, const std::vector<std::string>& row);
		void deleteRow   (int row);
		void addColumn   (const std::string& name, const std::vector<std::string>& column);
		void insertColumn(int column_index, const std::string& name, const std::vector<std::string>& column);
		void deleteColumn(int column);

		std::string getTableItem (int row, int column) const;
		PyObject*   getSelections() const;

		virtual void      setPyValue(PyObject* value) override;
		virtual PyObject* getPyValue() const override;
		virtual void      draw      () override;

	private:

		bool isIndexValid(int row, int column) const;
		void updateHashValues();

	private:

		std::map<std::pair<int, int>, bool>   m_selections;
		std::vector<std::string>              m_headers;
		std::vector<std::vector<std::string>> m_hashValues;
		std::vector<std::vector<std::string>> m_values;
		int                                   m_columns;

	};

}