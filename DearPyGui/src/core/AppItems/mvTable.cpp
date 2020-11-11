#include "mvTable.h"
#include "mvPythonTranslator.h"
#include "mvApp.h"
#include "mvAppLog.h"
#include "core/mvDataStorage.h"
#include "core/mvPythonExceptions.h"

namespace Marvel {

	mvTable::mvTable(const std::string& name, const std::vector<std::string>& headers)
		: mvAppItem(name)
	{
		m_height = 200;
		m_headers = headers;
		m_columns = headers.size();
	}

	bool mvTable::isIndexValid(int row, int column) const
	{
		if (column < 0 || row < 0)
		{
			ThrowPythonException("Table index must be a positive integer.");
			return false;
		}

		if (static_cast<size_t>(column) > m_columns + 1 || static_cast<size_t>(row) > m_values.size() + 1)
		{
			ThrowPythonException("Table indices out of range.");
			return false;
		}

		return true;
	}

	void mvTable::updateHashValues()
	{
		m_hashValues.clear();

		m_hashValues = m_values;

		for (size_t i = 0; i < m_hashValues.size(); i++)
		{
			for (size_t j = 0; j < m_hashValues[i].size(); j++)
				m_hashValues[i][j] = m_hashValues[i][j] + "##" + m_name + "-" + std::to_string(i) + "-" + std::to_string(j);

			for (size_t j = m_hashValues[i].size(); j < m_columns; j++)
				m_hashValues[i].push_back("##" + m_name + "-" + std::to_string(i) + "-" + std::to_string(j));
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
		auto values = ToVectVectString(value, m_name + " requires a list/tuple or list/tuple of strings.");

		m_values = values;

		if (!m_values.empty()) // just in case value is set to an empty list
		{
			while (m_headers.size() > m_columns)
				m_headers.pop_back();
		}

		updateHashValues();

		m_selections.clear();
	}

	PyObject* mvTable::getPyValue() const
	{
		return ToPyList(m_values);
	}

	PyObject* mvTable::getSelections() const
	{
		int selectionCount = 0;

		for (auto& item : m_selections)
			if (item.second) selectionCount++;

		std::vector<std::pair<int, int>> selections;

		for (auto& item : m_selections)
		{
			if (item.second)
				selections.emplace_back(item.first.first, item.first.second);
		}
		
		return ToPyList(selections);
	}

	void mvTable::addHeaders(const std::vector<std::string>& headers) 
	{
		m_headers = headers; 
		m_columns = headers.size(); 

		if (m_values.empty())
			return;

		if (m_columns > m_values[0].size())
		{
			for (auto& row : m_values)
				row.emplace_back("");
		}
		else if (m_columns < m_values[0].size())
		{
			m_columns = m_values[0].size();
			while (m_headers.size() < m_columns)
				m_headers.emplace_back("Header");
		}
		updateHashValues();
	}

	void mvTable::addRow(const std::vector<std::string>& row)
	{
		m_values.push_back(row);

		while (m_values.back().size() < m_headers.size())
			m_values.back().emplace_back("");

		while (m_values.back().size() > m_headers.size())
			m_values.back().pop_back();

		updateHashValues();
	}

	void mvTable::addColumn(const std::string& name, const std::vector<std::string>& column)
	{
		m_headers.push_back(name);

		if (column.size() > m_values.size())
		{
			for (unsigned i = 0; i < column.size(); i++)
			{
				// row exists
				if (i < m_values.size())
				{
					m_values[i].emplace_back(column[i]);
					continue;
				}

				// row does not exist
				m_values.push_back({});
				for (unsigned j = 0; j < m_headers.size() - 1; j++)
					m_values[i].emplace_back("");

				m_values[i].emplace_back(column[i]);
			}
		}

		else
		{
			size_t index = 0;
			for (auto& row : m_values)
			{
				if (index >= column.size())
				{
					row.emplace_back("");
					index++;
					continue;

				}

				row.push_back(column[index]);
				index++;
			}
		}

		m_columns++;

		updateHashValues();
	}

	void mvTable::insertColumn(int column_index, const std::string& name, const std::vector<std::string>& column)
	{
		if (!isIndexValid(0, column_index))
			return;

		if (static_cast<size_t>(column_index) == m_headers.size())
		{
			addColumn(name, column);
			return;
		}

		auto oldValues = m_values;
		auto oldHeaders = m_headers;

		m_values.clear();
		m_headers.clear();
		m_columns++;

		for (size_t i = 0; i < m_columns; i++)
		{
			if (i < static_cast<size_t>(column_index))
				m_headers.push_back(oldHeaders[i]);
			else if (i == static_cast<size_t>(column_index))
				m_headers.push_back(name);
			else
				m_headers.push_back(oldHeaders[i - 1]);
		}

		if (column.size() > oldValues.size())
		{
			for (size_t i = 0; i < column.size(); i++)
			{
				if (i < oldValues.size())
				{
					std::vector<std::string> row;
					for (size_t j = 0; j < oldHeaders.size(); j++)
					{
						if (j == static_cast<size_t>(column_index))
						{
							row.push_back(column[i]);
							continue;
						}

						if (j > static_cast<size_t>(column_index))
							row.push_back(oldValues[i][j - 1]);

						else
							row.push_back(oldValues[i][j]);

					}
					row.push_back(oldValues[i].back());

					m_values.push_back(row);
				}

				else
				{
					std::vector<std::string> row;
					for (size_t j = 0; j < oldHeaders.size(); j++)
					{
						if (j == static_cast<size_t>(column_index))
						{
							row.push_back(column[i]);
							continue;
						}
						row.push_back("");
					}
					row.push_back("");

					m_values.push_back(row);
				}
			}
		}

		else
		{
			for (size_t i = 0; i < oldValues.size(); i++)
			{
				std::vector<std::string> row;
				for (size_t j = 0; j < oldHeaders.size(); j++)
				{
					if (j == static_cast<size_t>(column_index))
					{
						if (i >= column.size())
							row.emplace_back("");
						else
							row.push_back(column[i]);
						continue;
					}

					if (j > static_cast<size_t>(column_index))
						row.push_back(oldValues[i][j - 1]);

					else
						row.push_back(oldValues[i][j]);

				}
				row.push_back(oldValues[i].back());

				m_values.push_back(row);
			}
		}

		updateHashValues();

		// update selections
		std::map<std::pair<int, int>, bool> oldSelection = m_selections;
		m_selections.clear();

		for (auto& item : oldSelection)
		{

			if (item.first.second >= column_index)
				m_selections[{item.first.first, item.first.second + 1}] = item.second;

			else
				m_selections[item.first] = item.second;

		}

	}

	void mvTable::insertRow(int row_index, const std::vector<std::string>& row)
	{
		if (!isIndexValid(row_index, 0))
			return;

		if (static_cast<size_t>(row_index) >= m_values.size())
		{
			addRow(row);
			return;
		}

		auto oldValues = m_values;

		m_values.clear();

		for (size_t i = 0; i < oldValues.size(); i++)
		{
			if (i == static_cast<size_t>(row_index))
			{
				m_values.push_back(row);
				continue;
			}

			if(i > static_cast<size_t>(row_index))
				m_values.push_back(oldValues[i-1]);
			else
				m_values.push_back(oldValues[i]);
		}
		m_values.push_back(oldValues.back());

		while (m_values[row_index].size() < m_headers.size())
			m_values[row_index].emplace_back("");

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

		if (static_cast<size_t>(column) >= m_headers.size())
		{
			ThrowPythonException("Column to delete does not exist.");
			return;
		}

		auto oldValues = m_values;
		auto oldHeaders = m_headers;

		m_values.clear();
		m_headers.clear();
		m_columns--;

		for (size_t i = 0; i < oldHeaders.size(); i++)
		{
			if (i == static_cast<size_t>(column))
				continue;
			m_headers.push_back(oldHeaders[i]);
		}

		//for (int i = 0; i < oldValues.size(); i++)
		for (auto& oldvalue : oldValues)
		{
			std::vector<std::string> row;
			for (size_t j = 0; j < oldHeaders.size(); j++)
			{
				if (j == static_cast<size_t>(column))
					continue;
				row.push_back(oldvalue[j]);
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

	void mvTable::clearTable()
	{
		m_selections.clear();
		m_hashValues.clear();
		m_values.clear();
	}

	void mvTable::draw()
	{
		auto styleManager = m_styleManager.getScopedStyleManager();

		ImGui::BeginChild(m_name.c_str(), ImVec2((float)m_width, (float)m_height));
		ImGui::Separator();
		if(m_columns > 0)
			ImGui::Columns((int)m_columns, nullptr, true);

		for (auto& header : m_headers)
		{
			ImGui::Text("%s", header.c_str());
			ImGui::NextColumn();
		}
		ImGui::Separator();

		int index = 0;
		for (size_t i = 0; i < m_hashValues.size(); i++)
		{
			for (size_t j = 0; j < m_columns; j++)
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
