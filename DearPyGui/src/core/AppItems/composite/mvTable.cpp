#include "mvTable.h"
#include "mvApp.h"
#include "mvAppLog.h"
#include "mvItemRegistry.h"
#include "mvThemeManager.h"

namespace Marvel {

	void mvTable::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_table", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::StringList, "headers"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before","This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Integer, "height","", "200"},
			{mvPythonDataType::Bool, "hide_headers", "Hide headers of the table", "False"},
			{mvPythonDataType::Bool, "show","Attempt to render", "True"}
		}, "Adds table.", "None", "Tables") });

		parsers->insert({ "set_table_data", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::ListStrList, "data"}
		}, "Overwrites table data.", "None", "Tables") });

		parsers->insert({ "get_table_data", mvPythonParser({
			{mvPythonDataType::String, "name"},
		}, "Gets table data.", "List[List[str]]", "Tables") });

		parsers->insert({ "set_headers", mvPythonParser({
			{mvPythonDataType::String, "table"},
			{mvPythonDataType::StringList, "headers"},
		}, "Sets a tables headers.", "None", "Tables") });

		parsers->insert({ "clear_table", mvPythonParser({
			{mvPythonDataType::String, "table"}
		}, "Clears data in a table", "None", "Tables") });

		parsers->insert({ "add_column", mvPythonParser({
			{mvPythonDataType::String, "table"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::StringList, "column"},
		}, "Adds a column to the end of a table.", "None", "Tables") });

		parsers->insert({ "insert_column", mvPythonParser({
			{mvPythonDataType::String, "table"},
			{mvPythonDataType::Integer, "column_index"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::StringList, "column"},
		}, "Inserts a column into a table.", "None", "Tables") });

		parsers->insert({ "delete_column", mvPythonParser({
			{mvPythonDataType::String, "table"},
			{mvPythonDataType::Integer, "column"}
		}, "Delete a column in a table.", "None", "Tables") });

		parsers->insert({ "add_row", mvPythonParser({
			{mvPythonDataType::String, "table"},
			{mvPythonDataType::StringList, "row"},
		}, "Adds a row to the end of a table.", "None", "Tables") });

		parsers->insert({ "insert_row", mvPythonParser({
			{mvPythonDataType::String, "table"},
			{mvPythonDataType::Integer, "row_index"},
			{mvPythonDataType::StringList, "row"},
		}, "Inserts a row into a table.", "None", "Tables") });

		parsers->insert({ "delete_row", mvPythonParser({
			{mvPythonDataType::String, "table"},
			{mvPythonDataType::Integer, "row"}
		}, "Delete a row in a table.", "None", "Tables") });

		parsers->insert({ "get_table_item", mvPythonParser({
			{mvPythonDataType::String, "table"},
			{mvPythonDataType::Integer, "row"},
			{mvPythonDataType::Integer, "column"}
		}, "Gets a table's cell value.", "str", "Tables") });

		parsers->insert({ "set_table_item", mvPythonParser({
			{mvPythonDataType::String, "table"},
			{mvPythonDataType::Integer, "row"},
			{mvPythonDataType::Integer, "column"},
			{mvPythonDataType::String, "value"},
		}, "Sets a table's cell value.", "None", "Tables") });

		parsers->insert({ "get_table_selections", mvPythonParser({
			{mvPythonDataType::String, "table"}
		}, "Retrieves data from storage.", "List[List[int]]", "Tables") });

		parsers->insert({ "set_table_selection", mvPythonParser({
			{mvPythonDataType::String, "table"},
			{mvPythonDataType::Integer, "row"},
			{mvPythonDataType::Integer, "column"},
			{mvPythonDataType::Bool, "value"},
		}, "Sets a table's cell selection value.", "None", "Tables") });
	}

	mvTable::mvTable(const std::string& name, const std::vector<std::string>& headers)
		: mvAppItem(name)
	{
		m_core_config.height = 200;
		m_headers = headers;
		m_columns = headers.size();
		m_hide_headers = false;
	}

	void mvTable::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "hide_headers")) m_hide_headers = ToBool(item);
	}

	void mvTable::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "hide_headers", ToPyBool(m_hide_headers));
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
				m_hashValues[i][j] = m_hashValues[i][j] + "##" + m_core_config.name + "-" + std::to_string(i) + "-" + std::to_string(j);

			for (size_t j = m_hashValues[i].size(); j < m_columns; j++)
				m_hashValues[i].push_back("##" + m_core_config.name + "-" + std::to_string(i) + "-" + std::to_string(j));
		}
	}

	void mvTable::setTableItem(int row, int column, const std::string& value)
	{
		if (!isIndexValid(row, column))
			return;

		m_values[row][column] = value;
		m_hashValues[row][column] = value + "##" + m_core_config.name + "-" + std::to_string(row) + "-" + std::to_string(column);

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
		auto values = ToVectVectString(value, m_core_config.name + " requires a list/tuple or list/tuple of strings.");

		m_values = std::move(values);

		if (!m_values.empty()) // just in case value is set to an empty list
		{
			while (m_headers.size() > m_columns)
				m_headers.pop_back();
		}

		updateHashValues();

		m_selections.clear();
	}

	PyObject* mvTable::getPyValue()
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
			
			if (index == row)
			{
				index++;
				continue;
			}
			m_values.push_back(item);

			index++;
			
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
		mvImGuiThemeScope scope(this);

		ImGui::BeginChild(m_core_config.name.c_str(), ImVec2((float)m_core_config.width, (float)m_core_config.height));
		ImGui::Separator();
		if(m_columns > 0)
			ImGui::Columns((int)m_columns, nullptr, true);
		
		if (!m_hide_headers) {
			for (auto& header : m_headers)
			{
				ImGui::Text("%s", header.c_str());
				ImGui::NextColumn();
			}
			ImGui::Separator();
		}

		ImVec4 alt_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_Header));
		alt_color.w = 0.10f;
		auto ralt_col = IM_COL32(alt_color.x * 255.0f, alt_color.y * 255.0f, alt_color.z * 255.0f, alt_color.w * 255.0f);

		int index = 0;
		for (size_t i = 0; i < m_hashValues.size(); i++)
		{

			for (size_t j = 0; j < m_columns; j++)
			{
				if (i % 2 == 0)
				{
					ImVec2 p_min = ImGui::GetCursorScreenPos();
					p_min.x = p_min.x - ImGui::GetStyle().ItemSpacing.x;
					p_min.y = p_min.y - ImGui::GetStyle().FramePadding.y;
					ImVec2 p_max = ImVec2(p_min.x + ImGui::GetWindowContentRegionWidth(), p_min.y + ImGui::GetFrameHeight());
					ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max, ralt_col);
				}

				if (ImGui::Selectable(m_hashValues[i][j].c_str(), m_selections[{i, j}]))
				{
					m_selections[{i, j}] = !m_selections[{i, j}];
					mvApp::GetApp()->getCallbackRegistry().addCallback(m_core_config.callback, m_core_config.name, m_core_config.callback_data);
				}
				ImGui::NextColumn();
				index++;
			}

		}

		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::EndChild();
	}

#ifdef MV_CPP
#else

	PyObject* add_table(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* headers;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* parent = "";
		const char* before = "";
		int width = 0;
		int height = 0;
		int show = true;
		bool hide_headers = false;

		if (!(mvApp::GetApp()->getParsers())["add_table"].parse(args, kwargs, __FUNCTION__,
			&name, &headers, &callback, &callback_data, &parent,
			&before, &width, &height, &show, &hide_headers))
			return ToPyBool(false);

		auto item = CreateRef<mvTable>(name, ToStringVect(headers));
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before);

		return GetPyNone();
	}

	PyObject* get_table_data(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if (!(mvApp::GetApp()->getParsers())["get_table_data"].parse(args, kwargs, __FUNCTION__, &name))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto item = mvApp::GetApp()->getItemRegistry().getItem(std::string(name));

		if (item == nullptr)
		{
			std::string message = name;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::mvTable)
		{
			std::string message = name;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		return static_cast<mvTable*>(item.get())->getPyValue();
	}

	PyObject* set_table_data(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* value;

		if (!(mvApp::GetApp()->getParsers())["set_table_data"].parse(args, kwargs, __FUNCTION__, &name, &value))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto item = mvApp::GetApp()->getItemRegistry().getItem(std::string(name));

		if (item == nullptr)
		{
			std::string message = name;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::mvTable)
		{
			std::string message = name;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		static_cast<mvTable*>(item.get())->setPyValue(value);

		return GetPyNone();
	}

	PyObject* set_headers(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		PyObject* headers;

		if (!(mvApp::GetApp()->getParsers())["set_headers"].parse(args, kwargs, __FUNCTION__, &table, &headers))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto item = mvApp::GetApp()->getItemRegistry().getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		auto prow = ToStringVect(headers);
		mvTable* atable = static_cast<mvTable*>(item.get());

		atable->addHeaders(prow);

		return GetPyNone();
	}

	PyObject* clear_table(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;

		if (!(mvApp::GetApp()->getParsers())["clear_table"].parse(args, kwargs, __FUNCTION__, &table))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto item = mvApp::GetApp()->getItemRegistry().getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::mvTable)
		{
			std::string message = table;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		mvTable* atable = static_cast<mvTable*>(item.get());
		atable->clearTable();
		Py_RETURN_NONE;
	}

	PyObject* get_table_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int row;
		int column;

		if (!(mvApp::GetApp()->getParsers())["get_table_item"].parse(args, kwargs, __FUNCTION__, &table, &row,
			&column))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto item = mvApp::GetApp()->getItemRegistry().getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::mvTable)
		{
			std::string message = table;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		mvTable* atable = static_cast<mvTable*>(item.get());
		return Py_BuildValue("s", atable->getTableItem(row, column).c_str());

	}

	PyObject* set_table_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int row;
		int column;
		const char* value;

		if (!(mvApp::GetApp()->getParsers())["set_table_item"].parse(args, kwargs, __FUNCTION__, &table, &row,
			&column, &value))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto item = mvApp::GetApp()->getItemRegistry().getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::mvTable)
		{
			std::string message = table;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		mvTable* atable = static_cast<mvTable*>(item.get());
		atable->setTableItem(row, column, value);

		return GetPyNone();
	}

	PyObject* get_table_selections(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;

		if (!(mvApp::GetApp()->getParsers())["get_table_selections"].parse(args, kwargs, __FUNCTION__, &table))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto item = mvApp::GetApp()->getItemRegistry().getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::mvTable)
		{
			std::string message = table;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		mvTable* atable = static_cast<mvTable*>(item.get());
		return atable->getSelections();
	}

	PyObject* set_table_selection(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int row;
		int column;
		int value;

		if (!(mvApp::GetApp()->getParsers())["set_table_selection"].parse(args, kwargs, __FUNCTION__, &table, &row,
			&column, &value))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto item = mvApp::GetApp()->getItemRegistry().getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::mvTable)
		{
			std::string message = table;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		mvTable* atable = static_cast<mvTable*>(item.get());
		atable->setSelection(row, column, value);

		return GetPyNone();
	}

	PyObject* add_column(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		const char* name;
		PyObject* column;

		if (!(mvApp::GetApp()->getParsers())["add_column"].parse(args, kwargs, __FUNCTION__, &table, &name, &column))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto item = mvApp::GetApp()->getItemRegistry().getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::mvTable)
		{
			std::string message = table;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		auto pcolumn = ToStringVect(column);

		mvTable* atable = static_cast<mvTable*>(item.get());
		atable->addColumn(name, pcolumn);

		return GetPyNone();
	}

	PyObject* insert_column(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int column_index;
		const char* name;
		PyObject* column;

		if (!(mvApp::GetApp()->getParsers())["insert_column"].parse(args, kwargs, __FUNCTION__, &table, &column_index, &name, &column))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto item = mvApp::GetApp()->getItemRegistry().getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::mvTable)
		{
			std::string message = table;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		auto prow = ToStringVect(column);

		mvTable* atable = static_cast<mvTable*>(item.get());
		atable->insertColumn(column_index, name, prow);

		return GetPyNone();
	}

	PyObject* delete_column(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int column;

		if (!(mvApp::GetApp()->getParsers())["delete_column"].parse(args, kwargs, __FUNCTION__, &table, &column))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto item = mvApp::GetApp()->getItemRegistry().getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::mvTable)
		{
			std::string message = table;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		mvTable* atable = static_cast<mvTable*>(item.get());
		atable->deleteColumn(column);

		return GetPyNone();
	}

	PyObject* add_row(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		PyObject* row;

		if (!(mvApp::GetApp()->getParsers())["add_row"].parse(args, kwargs, __FUNCTION__, &table, &row))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto item = mvApp::GetApp()->getItemRegistry().getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::mvTable)
		{
			std::string message = table;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		auto prow = ToStringVect(row);
		mvTable* atable = static_cast<mvTable*>(item.get());
		if (atable->getColumnCount() == 0)
		{
			std::vector<std::string> headers;
			for (size_t i = 0; i < prow.size(); i++)
				headers.emplace_back("Header" + std::to_string(i));
			atable->addHeaders(headers);
		}
		atable->addRow(prow);

		return GetPyNone();
	}

	PyObject* insert_row(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int row_index;
		PyObject* row;

		if (!(mvApp::GetApp()->getParsers())["insert_row"].parse(args, kwargs, __FUNCTION__, &table, &row_index, &row))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto item = mvApp::GetApp()->getItemRegistry().getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::mvTable)
		{
			std::string message = table;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		auto prow = ToStringVect(row);
		mvTable* atable = static_cast<mvTable*>(item.get());
		if (atable->getColumnCount() == 0)
		{
			std::vector<std::string> headers;
			for (size_t i = 0; i < prow.size(); i++)
				headers.emplace_back("Header" + std::to_string(i));
			atable->addHeaders(headers);
		}
		atable->insertRow(row_index, prow);

		return GetPyNone();
	}

	PyObject* delete_row(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int row;

		if (!(mvApp::GetApp()->getParsers())["delete_row"].parse(args, kwargs, __FUNCTION__, &table, &row))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto item = mvApp::GetApp()->getItemRegistry().getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::mvTable)
		{
			std::string message = table;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		mvTable* atable = static_cast<mvTable*>(item.get());
		atable->deleteRow(row);

		return GetPyNone();
	}

#endif // !MV_CPP
}
