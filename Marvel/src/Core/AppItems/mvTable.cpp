#include "mvTable.h"
#include "Core/mvApp.h"
#include "Core/StandardWindows/mvAppLog.h"

namespace Marvel {

	mvTable::mvTable(const std::string& parent, const std::string& name, const std::vector<std::string>& headers)
		: mvAppItem(parent, name)
	{
		m_headers = headers;
		m_columns = headers.size();
	}

	bool mvTable::isIndexValid(int row, int column) const
	{
		if (column < 0 || row < 0)
		{
			mvAppLog::getLogger()->LogError("Table index must be a positive integer.");
			return false;
		}

		if (column > m_columns + 1 || row > m_values.size() + 1)
		{
			mvAppLog::getLogger()->LogError("Table indices out of range.");
			return false;
		}

		return true;
	}

	void mvTable::updateHashValues()
	{
		m_hashValues.clear();

		m_hashValues = m_values;

		for (int i = 0; i < m_hashValues.size(); i++)
		{
			for (int j = 0; j < m_hashValues[i].size(); j++)
				m_hashValues[i][j] = m_hashValues[i][j] + "##" + m_name + "-" + std::to_string(i) + "-" + std::to_string(j);
		}
	}

	void mvTable::setTableItem(int row, int column, const std::string& value)
	{
		if (!isIndexValid(row, column))
			return;

		m_values[row][column] = value;
		m_hashValues[row][column] = value + "##" + m_name + "-" + std::to_string(row) + "-" + std::to_string(column);

	}

	std::string mvTable::getTableItem(int row, int column) const
	{
		if (!isIndexValid(row, column))
			return "";

		return m_values[row][column];
	}

	void mvTable::setSelection(int row, int column, bool value)
	{
		if (!isIndexValid(row, column))
			return;

		m_selections[{row, column}] = value;
	}

	void mvTable::setPyValue(PyObject* value)
	{
		PyGILState_STATE gstate = PyGILState_Ensure();

		if (!PyList_Check(value))
		{
			PyGILState_Release(gstate);
			mvAppLog::getLogger()->LogError(m_name + " type must be an list.");
			return;
		}

		std::vector<std::vector<std::string>> values;

		for (int i = 0; i < PyList_Size(value); i++)
		{
			std::vector<std::string> row;
			PyObject* prow = PyList_GetItem(value, i);
			for (int j = 0; j < PyList_Size(prow); j++)
			{
				PyObject* pitem = PyList_GetItem(prow, j);
				row.push_back(std::string(PyUnicode_AsUTF8(pitem)));
			}
			values.push_back(row);
		}

		PyGILState_Release(gstate);

		m_values = values;

		while (m_headers.size() > m_values[0].size())
			m_headers.pop_back();

		updateHashValues();

		m_selections.clear();
	}

	PyObject* mvTable::getPyValue() const
	{
		PyGILState_STATE gstate = PyGILState_Ensure();

		PyObject* pvalue = PyList_New(m_hashValues.size());

		for (int i = 0; i < m_hashValues.size(); i++)
		{
			PyObject* prow = PyList_New(m_hashValues[i].size());
			for (int j = 0; j < m_hashValues[i].size(); j++)
				PyList_SetItem(prow, j, PyUnicode_FromString(m_values[i][j].c_str()));
			PyList_SetItem(pvalue, i, prow);
		}

		PyGILState_Release(gstate);
		return pvalue;
	}

	PyObject* mvTable::getSelections() const
	{
		int selectionCount = 0;

		for (auto& item : m_selections)
			if (item.second) selectionCount++;

		PyGILState_STATE gstate = PyGILState_Ensure();

		PyObject* selections = PyList_New(selectionCount);
		int index = 0;
		for (auto& item : m_selections)
		{
			if (item.second)
			{
				PyObject* selection = PyList_New(2);
				PyList_SetItem(selection, 0, PyLong_FromLong(item.first.first));
				PyList_SetItem(selection, 1, PyLong_FromLong(item.first.second));
				PyList_SetItem(selections, index, selection);
				index++;
			}
		}
		PyGILState_Release(gstate);
		return selections;
	}

	void mvTable::addRow(const std::vector<std::string>& row)
	{
		m_values.push_back(row);

		while (m_values.back().size() < m_headers.size())
			m_values.back().push_back("");

		while (m_values.back().size() > m_headers.size())
			m_values.back().pop_back();

		updateHashValues();
	}

	void mvTable::addColumn(const std::string& name, const std::vector<std::string>& column)
	{
		m_headers.push_back(name);

		int index = 0;
		for (auto& row : m_values)
		{
			if (index >= column.size())
			{
				row.push_back("");
				continue;
				index++;
			}

			row.push_back(column[index]);
			index++;
		}

		m_columns++;

		updateHashValues();
	}

	void mvTable::insertColumn(int column_index, const std::string& name, const std::vector<std::string>& column)
	{
		if (!isIndexValid(0, column_index))
			return;

		if (column_index == m_headers.size())
		{
			addColumn(name, column);
			return;
		}

		auto oldValues = m_values;
		auto oldHeaders = m_headers;

		m_values.clear();
		m_headers.clear();
		m_columns++;

		for (int i = 0; i < m_columns; i++)
		{
			if (i < column_index)
				m_headers.push_back(oldHeaders[i]);
			else if (i == column_index)
				m_headers.push_back(name);
			else
				m_headers.push_back(oldHeaders[i - 1]);
		}

		for (int i = 0; i < oldValues.size(); i++)
		{
			std::vector<std::string> row;
			for (int j = 0; j < oldHeaders.size(); j++)
			{
				if (j == column_index)
				{
					if (i >= column.size())
						row.push_back("");
					else
						row.push_back(column[i]);
					continue;
				}

				if (j > column_index)
					row.push_back(oldValues[i][j - 1]);

				else
					row.push_back(oldValues[i][j]);

			}
			row.push_back(oldValues[i].back());

			m_values.push_back(row);
		}

		updateHashValues();

		// update selections
		std::map<std::pair<int, int>, bool> oldSelection = m_selections;
		m_selections.clear();

		for (auto& item : oldSelection)
		{

			if (item.first.second >= column_index)
				m_selections[{item.first.first, item.first.second+1}] = item.second;

			else
				m_selections[item.first] = item.second;

		}

	}

	void mvTable::insertRow(int row_index, const std::vector<std::string>& row)
	{
		if (!isIndexValid(row_index, 0))
			return;

		if (row_index == m_hashValues.size())
		{
			addRow(row);
			return;
		}

		auto oldValues = m_hashValues;

		m_values.clear();

		for (int i = 0; i < oldValues.size(); i++)
		{
			if (i == row_index)
			{
				m_values.push_back(row);
				continue;
			}

			if(i > row_index)
				m_values.push_back(oldValues[i-1]);
			else
				m_values.push_back(oldValues[i]);
		}
		m_values.push_back(oldValues.back());

		while (m_values[row_index].size() < m_headers.size())
			m_values[row_index].push_back("");

		while (m_values.back().size() > m_headers.size())
			m_values.back().pop_back();

		updateHashValues();

		// update selections
		std::map<std::pair<int, int>, bool> oldSelection = m_selections;
		m_selections.clear();

		for (auto& item : oldSelection)
		{

			if (item.first.first >= row_index)
				m_selections[{item.first.first + 1, item.first.second}] = item.second;

			else
				m_selections[item.first] = item.second;
			
		}

	}

	void mvTable::deleteRow(int row)
	{
		if (!isIndexValid(row, 0))
			return ;

		auto oldValues = m_values;

		m_values.clear();

		int index = 0;
		for (auto& item : oldValues)
		{
			index++;
			if (index == row)
				continue;
			m_values.push_back(item);
			
		}

		updateHashValues();

		// update selections
		std::map<std::pair<int, int>, bool> oldSelection = m_selections;
		m_selections.clear();

		for (auto& item : oldSelection)
		{
			if (item.first.first == row)
				continue;

			else if (item.first.first < row)
				m_selections[item.first] = item.second;


			else if (item.first.first > row)
				m_selections[{item.first.first - 1, item.first.second}] = item.second;
		}

	}

	void mvTable::deleteColumn(int column)
	{
		if (!isIndexValid(0, column))
			return;

		if (column >= m_headers.size())
		{
			mvAppLog::getLogger()->LogError("Column to delete does not exist.");
			return;
		}

		auto oldValues = m_values;
		auto oldHeaders = m_headers;

		m_values.clear();
		m_headers.clear();
		m_columns--;

		for (int i = 0; i < oldHeaders.size(); i++)
		{
			if (i == column)
				continue;
			m_headers.push_back(oldHeaders[i]);
		}

		for (int i = 0; i < oldValues.size(); i++)
		{
			std::vector<std::string> row;
			for (int j = 0; j < oldHeaders.size(); j++)
			{
				if (j == column)
					continue;
				row.push_back(oldValues[i][j]);
			}

			m_values.push_back(row);
		}

		updateHashValues();

		// update selections
		std::map<std::pair<int, int>, bool> oldSelection = m_selections;
		m_selections.clear();
		for (auto& item : oldSelection)
		{
			if (item.first.second == column)
				continue;

			else if (item.first.second < column)
				m_selections[item.first] = item.second;


			else if (item.first.second > column)
				m_selections[{item.first.first, item.first.second - 1}] = item.second;
		}

	}

	void mvTable::draw()
	{
		ImGui::BeginChild(m_name.c_str(), ImVec2(m_width, m_height));
		ImGui::Separator();
		ImGui::Columns(m_columns, 0, true);

		for (auto& header : m_headers)
		{
			ImGui::Text(header.c_str());
			ImGui::NextColumn();
		}
		ImGui::Separator();

		int index = 0;
		for (int i = 0; i < m_hashValues.size(); i++)
		{
			for (int j = 0; j < m_columns; j++)
			{
				if (ImGui::Selectable(m_hashValues[i][j].c_str(), m_selections[{i, j}]))
				{
					m_selections[{i, j}] = !m_selections[{i, j}];
					mvApp::GetApp()->runCallback(m_callback, m_name);
				}
				ImGui::NextColumn();
				index++;
			}
		}

		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::EndChild();

	}

}