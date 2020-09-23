#include "mvTableInterface.h"
#include "mvInterfaceCore.h"

namespace Marvel{

	void AddTableCommands(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_table", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::StringList, "headers"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before","This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height",""}
			{mvPythonDataType::Bool, "show",""}
		}, "Adds table.", "None", "Tables") });

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

		if (!(*mvApp::GetApp()->getParsers())["add_table"].parse(args, kwargs, __FUNCTION__, 
			&name, &headers, &callback, &callback_data, &parent,
			&before, &width, &height, &show))
			return ToPyBool(false);

		mvAppItem* item = new mvTable(name, ToStringVect(headers));
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		
		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* set_headers(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		PyObject* headers;

		if (!(*mvApp::GetApp()->getParsers())["set_headers"].parse(args, kwargs, __FUNCTION__, &table, &headers))
			return GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		auto prow = ToStringVect(headers);
		mvTable* atable = static_cast<mvTable*>(item);

		atable->addHeaders(prow);

		return GetPyNone();
	}

	PyObject* clear_table(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;

		if (!(*mvApp::GetApp()->getParsers())["clear_table"].parse(args, kwargs, __FUNCTION__, &table))
			return GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::Table)
		{
			std::string message = table;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		mvTable* atable = static_cast<mvTable*>(item);
		atable->clearTable();
		Py_RETURN_NONE;
	}

	PyObject* get_table_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int row;
		int column;

		if (!(*mvApp::GetApp()->getParsers())["get_table_item"].parse(args, kwargs, __FUNCTION__, &table, &row,
			&column))
			return GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::Table)
		{
			std::string message = table;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		mvTable* atable = static_cast<mvTable*>(item);
		return Py_BuildValue("s", atable->getTableItem(row, column).c_str());

	}

	PyObject* set_table_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int row;
		int column;
		const char* value;

		if (!(*mvApp::GetApp()->getParsers())["set_table_item"].parse(args, kwargs, __FUNCTION__, &table, &row,
			&column, &value))
			return GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::Table)
		{
			std::string message = table;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		mvTable* atable = static_cast<mvTable*>(item);
		atable->setTableItem(row, column, value);

		return GetPyNone();
	}

	PyObject* get_table_selections(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;

		if (!(*mvApp::GetApp()->getParsers())["get_table_selections"].parse(args, kwargs, __FUNCTION__, &table))
			return GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::Table)
		{
			std::string message = table;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		mvTable* atable = static_cast<mvTable*>(item);
		return atable->getSelections();
	}

	PyObject* set_table_selection(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int row;
		int column;
		int value;

		if (!(*mvApp::GetApp()->getParsers())["set_table_selection"].parse(args, kwargs, __FUNCTION__, &table, &row,
			&column, &value))
			return GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::Table)
		{
			std::string message = table;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		mvTable* atable = static_cast<mvTable*>(item);
		atable->setSelection(row, column, value);

		return GetPyNone();
	}

	PyObject* add_column(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		const char* name;
		PyObject* column;

		if (!(*mvApp::GetApp()->getParsers())["add_column"].parse(args, kwargs, __FUNCTION__, &table, &name, &column))
			return GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::Table)
		{
			std::string message = table;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		auto pcolumn = ToStringVect(column);

		mvTable* atable = static_cast<mvTable*>(item);
		atable->addColumn(name, pcolumn);

		return GetPyNone();
	}

	PyObject* insert_column(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int column_index;
		const char* name;
		PyObject* column;

		if (!(*mvApp::GetApp()->getParsers())["insert_column"].parse(args, kwargs, __FUNCTION__, &table, &column_index, &name, &column))
			return GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::Table)
		{
			std::string message = table;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		auto prow = ToStringVect(column);

		mvTable* atable = static_cast<mvTable*>(item);
		atable->insertColumn(column_index, name, prow);

		return GetPyNone();
	}

	PyObject* delete_column(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int column;

		if (!(*mvApp::GetApp()->getParsers())["delete_column"].parse(args, kwargs, __FUNCTION__, &table, &column))
			return GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::Table)
		{
			std::string message = table;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		mvTable* atable = static_cast<mvTable*>(item);
		atable->deleteColumn(column);

		return GetPyNone();
	}

	PyObject* add_row(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		PyObject* row;

		if (!(*mvApp::GetApp()->getParsers())["add_row"].parse(args, kwargs, __FUNCTION__, &table, &row))
			return GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::Table)
		{
			std::string message = table;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		auto prow = ToStringVect(row);
		mvTable* atable = static_cast<mvTable*>(item);
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

		if (!(*mvApp::GetApp()->getParsers())["insert_row"].parse(args, kwargs, __FUNCTION__, &table, &row_index, &row))
			return GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::Table)
		{
			std::string message = table;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		auto prow = ToStringVect(row);
		mvTable* atable = static_cast<mvTable*>(item);
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

		if (!(*mvApp::GetApp()->getParsers())["delete_row"].parse(args, kwargs, __FUNCTION__, &table, &row))
			return GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			ThrowPythonException(message + " table does not exist.");
			return GetPyNone();
		}

		if (item->getType() != mvAppItemType::Table)
		{
			std::string message = table;
			ThrowPythonException(message + " is not a table.");
			return GetPyNone();
		}

		mvTable* atable = static_cast<mvTable*>(item);
		atable->deleteRow(row);

		return GetPyNone();
	}
}