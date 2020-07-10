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

	void mvTable::setTableItem(int row, int column, const std::string& value)
	{
		if (column > m_columns + 1 || row > m_textOrignalValues.size() + 1)
		{
			mvAppLog::getLogger()->LogError(m_name + " item out of index.");
			return;
		}

		if (column < 0 || row < 0)
		{
			mvAppLog::getLogger()->LogError(m_name + " item out of index.");
			return;
		}

		m_textOrignalValues[row][column] = value;
		m_textValues[row][column] = value + "##" + m_name + "-" + std::to_string(row) + "-" + std::to_string(column);

	}

	std::string mvTable::getTableItem(int row, int column) const
	{
		if (column > m_columns + 1 || row > m_textOrignalValues.size() + 1)
		{
			mvAppLog::getLogger()->LogError(m_name + " item out of index.");
			return "";
		}

		if (column < 0 || row < 0)
		{
			mvAppLog::getLogger()->LogError(m_name + " item out of index.");
			return "";
		}

		return m_textOrignalValues[row][column];
	}

	void mvTable::setSelection(int row, int column, bool value)
	{
		if (column > m_columns + 1 || row > m_textOrignalValues.size() + 1)
		{
			mvAppLog::getLogger()->LogError(m_name + " item out of index.");
			return;
		}

		if (column < 0 || row < 0)
		{
			mvAppLog::getLogger()->LogError(m_name + " item out of index.");
			return;
		}

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

		std::vector<std::vector<std::string>> textValues;

		for (int i = 0; i < PyList_Size(value); i++)
		{
			std::vector<std::string> row;
			PyObject* prow = PyList_GetItem(value, i);
			for (int j = 0; j < PyList_Size(prow); j++)
			{
				PyObject* pitem = PyList_GetItem(prow, j);
				row.push_back(std::string(PyUnicode_AsUTF8(pitem)));
			}
			textValues.push_back(row);
		}

		PyGILState_Release(gstate);

		m_textValues = textValues;
		m_textOrignalValues = textValues;

		while (m_headers.size() > m_textValues[0].size())
			m_headers.pop_back();

		for (int i = 0; i < m_textValues.size(); i++)
		{
			for (int j = 0; j < m_textValues[i].size(); j++)
				m_textValues[i][j] = m_textValues[i][j] + "##" + m_name + "-" + std::to_string(i) + "-" + std::to_string(j);
		}

		m_selections.clear();
	}

	PyObject* mvTable::getPyValue() const
	{
		PyGILState_STATE gstate = PyGILState_Ensure();

		PyObject* pvalue = PyList_New(m_textValues.size());

		for (int i = 0; i < m_textValues.size(); i++)
		{
			PyObject* prow = PyList_New(m_textValues[i].size());
			for (int j = 0; j < m_textValues[i].size(); j++)
				PyList_SetItem(prow, j, PyUnicode_FromString(m_textOrignalValues[i][j].c_str()));
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
		m_textValues.push_back(row);
		m_textOrignalValues.push_back(row);

		while (m_textValues.back().size() < m_headers.size())
			m_textValues.back().push_back("");

		while (m_textOrignalValues.back().size() < m_headers.size())
			m_textOrignalValues.back().push_back("");

		while (m_textOrignalValues.back().size() > m_headers.size())
			m_textOrignalValues.back().pop_back();

		while (m_textValues.back().size() > m_headers.size())
			m_textValues.back().pop_back();

		for (int i = 0; i < m_headers.size(); i++)
			m_textValues.back()[i] = m_textValues.back()[i] + "##" + m_name + "-" + std::to_string(m_textValues.size() - 1) + "-" + std::to_string(i);

	}

	void mvTable::addColumn(const std::string& name, const std::vector<std::string>& column)
	{
		m_headers.push_back(name);

		int index = 0;
		for (auto& row : m_textOrignalValues)
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

		m_textValues = m_textOrignalValues;
		m_columns++;

		for (int i = 0; i < m_textValues.size(); i++)
		{
			for (int j = 0; j < m_textValues[i].size(); j++)
				m_textValues[i][j] = m_textValues[i][j] + "##" + m_name + "-" + std::to_string(i) + "-" + std::to_string(j);
		}

	}

	void mvTable::insertColumn(int column_index, const std::string& name, const std::vector<std::string>& column)
	{
		if (column_index > m_headers.size() || column_index < 0)
		{
			mvAppLog::getLogger()->LogError(m_name + " column index out of range.");
			return;
		}

		if (column_index == m_headers.size())
		{
			addColumn(name, column);
			return;
		}

		auto oldValues = m_textValues;
		auto oldHeaders = m_headers;

		m_textOrignalValues.clear();
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

			m_textOrignalValues.push_back(row);
		}

		m_textValues = m_textOrignalValues;

		for (int i = 0; i < m_textValues.size(); i++)
		{

			for (int j = 0; j < m_textValues[i].size(); j++)
				m_textValues[i][j] = m_textValues[i][j] + "##" + m_name + "-" + std::to_string(i) + "-" + std::to_string(j);
		}

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
		if (row_index > m_textValues.size() || row_index < 0)
		{
			mvAppLog::getLogger()->LogError(m_name + " row index out of range.");
			return;
		}

		if (row_index == m_textValues.size())
		{
			addRow(row);
			return;
		}

		auto oldValues = m_textValues;

		m_textOrignalValues.clear();

		for (int i = 0; i < oldValues.size(); i++)
		{
			if (i == row_index)
			{
				m_textOrignalValues.push_back(row);
				continue;
			}

			if(i > row_index)
				m_textOrignalValues.push_back(oldValues[i-1]);
			else
				m_textOrignalValues.push_back(oldValues[i]);
		}
		m_textOrignalValues.push_back(oldValues.back());

		while (m_textOrignalValues[row_index].size() < m_headers.size())
			m_textOrignalValues[row_index].push_back("");

		while (m_textOrignalValues.back().size() > m_headers.size())
			m_textOrignalValues.back().pop_back();

		m_textValues = m_textOrignalValues;

		for (int i = 0; i < m_textValues.size(); i++)
		{

			for (int j = 0; j < m_textValues[i].size(); j++)
				m_textValues[i][j] = m_textValues[i][j] + "##" + m_name + "-" + std::to_string(i) + "-" + std::to_string(j);
		}

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
		if (row >= m_textValues.size() || row < 0)
		{
			mvAppLog::getLogger()->LogError(m_name + " row index out of range.");
			return;
		}

		auto oldValues = m_textValues;

		m_textValues.clear();
		m_textOrignalValues.clear();

		int index = 0;
		for (auto& item : oldValues)
		{
			index++;
			if (index == row)
				continue;
			m_textValues.push_back(item);
			
		}

		for (int i = 0; i < m_textValues.size(); i++)
		{
			
			for (int j = 0; j < m_textValues[i].size(); j++)
				m_textValues[i][j] = m_textValues[i][j] + "##" + m_name + "-" + std::to_string(i) + "-" + std::to_string(j);
		}

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
		if (column >= m_textValues.size() || column < 0)
		{
			mvAppLog::getLogger()->LogError(m_name + " column index out of range.");
			return;
		}

		auto oldValues = m_textValues;
		auto oldHeaders = m_headers;

		m_textOrignalValues.clear();
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

			m_textOrignalValues.push_back(row);
		}

		m_textValues = m_textOrignalValues;

		for (int i = 0; i < m_textValues.size(); i++)
		{
			for (int j = 0; j < m_textValues[i].size(); j++)
				m_textValues[i][j] = m_textValues[i][j] + "##" + m_name + "-" + std::to_string(i) + "-" + std::to_string(j);
		}

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
		ImGui::Separator();
		ImGui::Columns(m_columns, 0, true);

		for (auto& header : m_headers)
		{
			ImGui::Text(header.c_str());
			ImGui::NextColumn();
		}
		ImGui::Separator();

		int index = 0;
		for (int i = 0; i < m_textValues.size(); i++)
		{
			for (int j = 0; j < m_columns; j++)
			{
				if (ImGui::Selectable(m_textValues[i][j].c_str(), m_selections[{i, j}]))
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

	}

}