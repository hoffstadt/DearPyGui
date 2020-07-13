#include "mvPythonModule.h"
#include "Core/mvApp.h"
#include "Core/StandardWindows/mvAppLog.h"
#include "mvPythonParser.h"
#include "mvPythonTranslator.h"
#include "Core/AppItems/mvAppItems.h"
#include "mvInterfaces.h"
#include "mvInterfaceRegistry.h"
#include "Core/mvUtilities.h"

namespace Marvel {

	static std::map<std::string, mvPythonParser> Parsers = mvInterfaceRegistry::GetRegistry()->getPythonInterface("sbApp");

	std::map<std::string, mvPythonParser>& BuildAppInterface() {

		std::map<std::string, mvPythonParser>* parsers = new std::map < std::string, mvPythonParser>();

		parsers->insert({ "run_async_function", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Object, "data"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "return_handler"},
		}, "Runs a function asyncronously.") });

		parsers->insert({ "open_file_dialog", mvPythonParser({
			{mvPythonDataType::StringList, "extensions", "i.e [['Python', '*.py']]"},
		}, "Opens an 'open file' dialog.", "str") });

		parsers->insert({ "save_file_dialog", mvPythonParser({
			{mvPythonDataType::StringList, "extensions", "i.e [['Python', '*.py']]"},
		}, "Opens an 'save file' dialog.", "str") });

		parsers->insert({ "add_data", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Object, "data"}
		}, "Adds data for later retrieval.") });

		parsers->insert({ "get_data", mvPythonParser({
			{mvPythonDataType::String, "name"}
		}, "Retrieves data from storage.", "object") });

		parsers->insert({ "set_table_item", mvPythonParser({
			{mvPythonDataType::String, "table"},
			{mvPythonDataType::Integer, "row"},
			{mvPythonDataType::Integer, "column"},
			{mvPythonDataType::String, "value"},
		}, "Sets a table's cell value.") });

		parsers->insert({ "delete_row", mvPythonParser({
			{mvPythonDataType::String, "table"},
			{mvPythonDataType::Integer, "row"}
		}, "Delete a row in a table.") });

		parsers->insert({ "delete_column", mvPythonParser({
			{mvPythonDataType::String, "table"},
			{mvPythonDataType::Integer, "column"}
		}, "Delete a column in a table.") });

		parsers->insert({ "add_row", mvPythonParser({
			{mvPythonDataType::String, "table"},
			{mvPythonDataType::StringList, "row"},
		}, "Adds a row to a table.") });

		parsers->insert({ "add_column", mvPythonParser({
			{mvPythonDataType::String, "table"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::StringList, "column"},
		}, "Adds a column to a table.") });

		parsers->insert({ "insert_column", mvPythonParser({
			{mvPythonDataType::String, "table"},
			{mvPythonDataType::Integer, "column_index"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::StringList, "column"},
		}, "Inserts a column into a table.") });

		parsers->insert({ "insert_row", mvPythonParser({
			{mvPythonDataType::String, "table"},
			{mvPythonDataType::Integer, "row_index"},
			{mvPythonDataType::StringList, "row"},
		}, "Inserts a row into a table.") });

		parsers->insert({ "set_table_selection", mvPythonParser({
			{mvPythonDataType::String, "table"},
			{mvPythonDataType::Integer, "row"},
			{mvPythonDataType::Integer, "column"},
			{mvPythonDataType::Bool, "value"},
		}, "Sets a table's cell selection value.") });

		parsers->insert({ "get_table_item", mvPythonParser({
			{mvPythonDataType::String, "table"},
			{mvPythonDataType::Integer, "row"},
			{mvPythonDataType::Integer, "column"}
		}, "Gets a table's cell value.", "str") });

		parsers->insert({ "get_table_selections", mvPythonParser({
			{mvPythonDataType::String, "table"}
		}, "Retrieves data from storage.", "List of integer pairs.") });

		parsers->insert({ "delete_data", mvPythonParser({
			{mvPythonDataType::String, "name"}
		}, "Deletes data from storage.") });

		parsers->insert({ "delete_item", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Bool, "children_only"}
		}, "Deletes an item if it exists.") });

		parsers->insert({ "move_item_up", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Moves item up if possible and if it exists.") });

		parsers->insert({ "move_item_down", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Moves down up if possible and if it exists.") });

		parsers->insert({ "get_style_item", mvPythonParser({
			{mvPythonDataType::Integer, "item"}
		}, "Returns a style item's value", "(float, float)") });

		parsers->insert({ "get_theme_item", mvPythonParser({
			{mvPythonDataType::Integer, "item"}
		}, "Returns a theme item's color", "(float, float, float, float)") });

		parsers->insert({ "get_item_callback", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns an item' callback", "str") });

		parsers->insert({ "get_item_height", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns an item's height.", "float") });

		parsers->insert({ "get_total_time", mvPythonParser({
		}, "Returns total time since app started.", "float") });

		parsers->insert({ "get_delta_time", mvPythonParser({
		}, "Returns time since last frame.", "float") });

		parsers->insert({ "get_item_width", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns an item's width.", "float") });

		parsers->insert({ "get_item_popup", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns an item's popup.", "str") });

		parsers->insert({ "get_item_tip", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns an item's tip.", "str") });

		parsers->insert({ "get_main_callback", mvPythonParser({
		}, "Returns the main callback.", "str") });

		parsers->insert({ "get_main_window_size", mvPythonParser({
		}, "Returns the size of the main window.", "(float, float)") });

		parsers->insert({ "get_theme", mvPythonParser({
		}, "Returns the current theme.", "str") });

		parsers->insert({ "get_thread_count", mvPythonParser({
		}, "Returns the allowable thread count.", "int") });

		parsers->insert({ "is_threadpool_high_performance", mvPythonParser({
		}, "Checks if the threadpool is allowed to use the maximum number of threads.", "Boolean") });

		parsers->insert({ "get_threadpool_timeout", mvPythonParser({
		}, "Returns the threadpool timeout in seconds.", "float") });

		parsers->insert({ "get_active_window", mvPythonParser({
		}, "Returns the active window name.", "str") });

		parsers->insert({ "get_marvel_version", mvPythonParser({
		}, "Returns the current version of Marvel.", "str") });

		parsers->insert({ "set_threadpool_timeout", mvPythonParser({
			{mvPythonDataType::Float, "time"}
		}, "Sets the threadpool timeout.") });

		parsers->insert({ "set_thread_count", mvPythonParser({
			{mvPythonDataType::Integer, "threads"}
		}, "Sets number of threads to use if the threadpool is active.") });

		parsers->insert({ "show_documentation", mvPythonParser({
		}, "Shows the documentation window.") });

		parsers->insert({ "show_about", mvPythonParser({
		}, "Shows the about window.") });

		parsers->insert({ "show_metrics", mvPythonParser({
		}, "Shows the metrics window.") });

		parsers->insert({ "show_debug", mvPythonParser({
		}, "Shows the debug window.") });

		parsers->insert({ "close_popup", mvPythonParser({
		}, "Needs documentation") });

		parsers->insert({ "show_source", mvPythonParser({
		}, "Shows the source code for the current app.") });

		parsers->insert({ "set_threadpool_high_performance", mvPythonParser({
		}, "Set the thread count to the maximum number of threads on your computer.") });

		parsers->insert({ "add_item_color_style", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::Integer, "style"},
			{mvPythonDataType::FloatList, "color"}
		}, "Needs documentation") });

		parsers->insert({ "is_item_hovered", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item is hovered.", "Boolean") });

		parsers->insert({ "set_main_window_size", mvPythonParser({
			{mvPythonDataType::Integer, "width"},
			{mvPythonDataType::Integer, "height"}
		}, "Sets the main window size.") });

		parsers->insert({ "is_item_active", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item is active.", "Boolean") });

		parsers->insert({ "is_item_focused", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item is focused.", "Boolean") });

		parsers->insert({ "is_item_clicked", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item is clicked.", "Boolean") });

		parsers->insert({ "is_item_visible", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item is visible.", "Boolean") });

		parsers->insert({ "is_item_edited", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item has been edited.", "Boolean") });

		parsers->insert({ "is_item_activated", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item has been activated.", "Boolean") });

		parsers->insert({ "is_item_deactivated", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item has been deactivated.", "Boolean") });

		parsers->insert({ "is_item_deactivated_after_edit", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item has been deactivated after editing.", "Boolean") });

		parsers->insert({ "is_item_toggled_open", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item is toggled.", "Boolean") });

		parsers->insert({ "get_item_rect_min", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Returns an item's minimum allowable size.", "(float, float)") });

		parsers->insert({ "get_item_rect_max", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Returns an item's maximum allowable size.", "(float, float)") });

		parsers->insert({ "get_item_rect_size", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Returns an item's current size.", "(float, float)") });

		parsers->insert({ "change_style_item", mvPythonParser({
			{mvPythonDataType::Integer, "item"},
			{mvPythonDataType::Float, "x"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Float, "y"}
		}, "Needs documentation") });

		parsers->insert({ "change_theme_item", mvPythonParser({
			{mvPythonDataType::Integer, "item"},
			{mvPythonDataType::Float, "r"},
			{mvPythonDataType::Float, "g"},
			{mvPythonDataType::Float, "b"},
			{mvPythonDataType::Float, "a"}
		}, "Needs documentation") });

		parsers->insert({ "get_value", mvPythonParser({
			{mvPythonDataType::String, "name"}
		}, "Returns an item's value or None if there is none.", "Depends") });

		parsers->insert({ "set_value", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Object, "value"}
		}, "Sets an item's value if applicable.") });

		parsers->insert({ "show_item", mvPythonParser({
			{mvPythonDataType::String, "name"}
		}, "Shows an item if it was hidden.") });

		parsers->insert({ "hide_item", mvPythonParser({
			{mvPythonDataType::String, "name"}
		}, "Hides an item.") });

		parsers->insert({ "set_main_callback", mvPythonParser({
			{mvPythonDataType::String, "callback"}
		}, "Sets the callback to be ran every frame.") });

		parsers->insert({ "set_item_callback", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::String, "callback"}
		}, "Sets an item's callback if applicable.") });

		parsers->insert({ "set_item_popup", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::String, "popup"}
		}, "Sets an item's popup if applicable.") });

		parsers->insert({ "set_item_tip", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::String, "tip"}
		}, "Sets a simple tooltip for an item.") });

		parsers->insert({ "set_item_width", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::Integer, "width"}
		}, "Sets an item's width.") });

		parsers->insert({ "set_item_height", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::Integer, "height"}
		}, "Sets an item's height if applicable.") });

		parsers->insert({ "set_theme", mvPythonParser({
			{mvPythonDataType::String, "theme"}
		}, "Set the application's theme to a built-in theme.") });

		return *parsers;
	}

	PyObject* set_table_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int row;
		int column;
		const char* value;

		if (!Parsers["set_table_item"].parse(args, kwargs, __FUNCTION__, &table, &row,
			&column, &value))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			mvAppLog::getLogger()->LogWarning(message + " table does not exist.");
			Py_RETURN_NONE;
		}

		mvTable* atable = static_cast<mvTable*>(item);
		atable->setTableItem(row, column, value);

		Py_RETURN_NONE;
	}

	PyObject* delete_row(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int row;

		if (!Parsers["delete_row"].parse(args, kwargs, __FUNCTION__, &table, &row))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			mvAppLog::getLogger()->LogWarning(message + " table does not exist.");
			Py_RETURN_NONE;
		}

		mvTable* atable = static_cast<mvTable*>(item);
		atable->deleteRow(row);

		Py_RETURN_NONE;
	}

	PyObject* delete_column(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int column;

		if (!Parsers["delete_column"].parse(args, kwargs, __FUNCTION__, &table, &column))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			mvAppLog::getLogger()->LogWarning(message + " table does not exist.");
			Py_RETURN_NONE;
		}

		mvTable* atable = static_cast<mvTable*>(item);
		atable->deleteColumn(column);

		Py_RETURN_NONE;
	}

	PyObject* add_row(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		PyObject* row;

		if (!Parsers["add_row"].parse(args, kwargs, __FUNCTION__, &table, &row))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			mvAppLog::getLogger()->LogWarning(message + " table does not exist.");
			Py_RETURN_NONE;
		}

		auto prow = mvPythonTranslator::getStringVec(row);

		mvTable* atable = static_cast<mvTable*>(item);
		atable->addRow(prow);

		Py_RETURN_NONE;
	}

	PyObject* add_column(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		const char* name;
		PyObject* column;

		if (!Parsers["add_column"].parse(args, kwargs, __FUNCTION__, &table, &name, &column))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			mvAppLog::getLogger()->LogWarning(message + " table does not exist.");
			Py_RETURN_NONE;
		}

		auto pcolumn = mvPythonTranslator::getStringVec(column);

		mvTable* atable = static_cast<mvTable*>(item);
		atable->addColumn(name, pcolumn);

		Py_RETURN_NONE;
	}

	PyObject* insert_row(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int row_index;
		PyObject* row;

		if (!Parsers["insert_row"].parse(args, kwargs, __FUNCTION__, &table, &row_index, &row))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			mvAppLog::getLogger()->LogWarning(message + " table does not exist.");
			Py_RETURN_NONE;
		}

		auto prow = mvPythonTranslator::getStringVec(row);

		mvTable* atable = static_cast<mvTable*>(item);
		atable->insertRow(row_index, prow);

		Py_RETURN_NONE;
	}

	PyObject* insert_column(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int column_index;
		const char* name;
		PyObject* column;

		if (!Parsers["insert_column"].parse(args, kwargs, __FUNCTION__, &table, &column_index, &name, &column))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			mvAppLog::getLogger()->LogWarning(message + " table does not exist.");
			Py_RETURN_NONE;
		}

		auto prow = mvPythonTranslator::getStringVec(column);

		mvTable* atable = static_cast<mvTable*>(item);
		atable->insertColumn(column_index, name, prow);

		Py_RETURN_NONE;
	}

	PyObject* set_table_selection(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int row;
		int column;
		int value;

		if (!Parsers["set_table_selection"].parse(args, kwargs, __FUNCTION__, &table, &row,
			&column, &value))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			mvAppLog::getLogger()->LogWarning(message + " table does not exist.");
			Py_RETURN_NONE;
		}

		mvTable* atable = static_cast<mvTable*>(item);
		atable->setSelection(row, column, value);

		Py_RETURN_NONE;
	}

	PyObject* get_table_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;
		int row;
		int column;

		if (!Parsers["get_table_item"].parse(args, kwargs, __FUNCTION__, &table, &row,
			&column))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			mvAppLog::getLogger()->LogWarning(message + " table does not exist.");
			Py_RETURN_NONE;
		}

		mvTable* atable = static_cast<mvTable*>(item);
		return Py_BuildValue("s", atable->getTableItem(row, column).c_str());

	}

	PyObject* get_table_selections(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* table;

		if (!Parsers["get_table_selections"].parse(args, kwargs, __FUNCTION__, &table))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(table);
		if (item == nullptr)
		{
			std::string message = table;
			mvAppLog::getLogger()->LogWarning(message + " table does not exist.");
			Py_RETURN_NONE;
		}

		mvTable* atable = static_cast<mvTable*>(item);
		return atable->getSelections();
	}

	PyObject* run_async_function(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* return_handler = "";
		PyObject* data;

		if (!Parsers["run_async_function"].parse(args, kwargs, __FUNCTION__, &name, &data, &return_handler))
			Py_RETURN_NONE;

		mvApp::GetApp()->addMTCallback(name, data, return_handler);

		Py_RETURN_NONE;

	}

	PyObject* delete_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (std::this_thread::get_id() != mvApp::GetApp()->getMainThreadID())
		{
			mvAppLog::getLogger()->LogWarning("Items can not be modified outside of the main thread.");
			Py_RETURN_NONE;
		}

		const char* item;
		int childrenOnly = false;

		if (!Parsers["delete_item"].parse(args, kwargs, __FUNCTION__, &item, &childrenOnly))
			Py_RETURN_NONE;

		if(childrenOnly)
			mvApp::GetApp()->deleteItemChildren(item);
		else
			mvApp::GetApp()->deleteItem(item);

		Py_RETURN_NONE;

	}

	PyObject* open_file_dialog(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* extensions;

		if (!Parsers["open_file_dialog"].parse(args, kwargs, __FUNCTION__, &extensions))
			Py_RETURN_NONE;

		std::string file = OpenFile(mvPythonTranslator::getStringPairVec(extensions));

		return Py_BuildValue("s", file.c_str());
	}

	PyObject* save_file_dialog(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* extensions;

		if (!Parsers["save_file_dialog"].parse(args, kwargs, __FUNCTION__, &extensions))
			Py_RETURN_NONE;

		std::string file = SaveFile(mvPythonTranslator::getStringPairVec(extensions));

		return Py_BuildValue("s", SaveFile(mvPythonTranslator::getStringPairVec(extensions)).c_str());
	}

	PyObject* move_item_up(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (std::this_thread::get_id() != mvApp::GetApp()->getMainThreadID())
		{
			mvAppLog::getLogger()->LogWarning("Items can not be modified outside of the main thread.");
			Py_RETURN_NONE;
		}

		const char* item;

		if (!Parsers["move_item_up"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		mvApp::GetApp()->moveItemUp(item);

		Py_RETURN_NONE;

	}

	PyObject* move_item_down(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (std::this_thread::get_id() != mvApp::GetApp()->getMainThreadID())
		{
			mvAppLog::getLogger()->LogWarning("Items can not be modified outside of the main thread.");
			Py_RETURN_NONE;
		}

		const char* item;

		if (!Parsers["move_item_down"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		mvApp::GetApp()->moveItemDown(item);

		Py_RETURN_NONE;

	}

	PyObject* get_style_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int item;

		if (!Parsers["get_style_item"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		auto values = mvApp::GetApp()->getStyleItem(item);

		return Py_BuildValue("(ff)", values.first, values.second);
	}

	PyObject* get_theme_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int item;

		if (!Parsers["get_theme_item"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		auto color = mvApp::GetApp()->getThemeItem(item);

		return Py_BuildValue("(iiii)", color.r, color.g, color.b, color.a);
	}

	PyObject* get_total_time(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return Py_BuildValue("f", mvApp::GetApp()->getTotalTime());
	}

	PyObject* get_delta_time(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return Py_BuildValue("f", mvApp::GetApp()->getDeltaTime());
	}

	PyObject* get_item_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["get_item_callback"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("s", appitem->getCallback());

		Py_RETURN_NONE;
	}

	PyObject* get_item_height(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["get_item_height"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->getHeight());

		Py_RETURN_NONE;
	}

	PyObject* get_item_width(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["get_item_width"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->getWidth());

		Py_RETURN_NONE;
	}

	PyObject* get_item_popup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["get_item_popup"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("s", appitem->getPopup());

		Py_RETURN_NONE;
	}

	PyObject* get_item_tip(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["get_item_tip"].parse(args, kwargs, __FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("s", appitem->getTip());

		Py_RETURN_NONE;
	}

	PyObject* get_main_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return Py_BuildValue("s", mvApp::GetApp()->getMainCallback());
	}

	PyObject* get_main_window_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return Py_BuildValue("(i,i)", mvApp::GetApp()->getWindowWidth(), mvApp::GetApp()->getWindowWidth());
	}

	PyObject* get_theme(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return Py_BuildValue("s", mvApp::GetApp()->getAppTheme());
	}

	PyObject* get_thread_count(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return Py_BuildValue("i", mvApp::GetApp()->getThreadCount());
	}

	PyObject* is_threadpool_high_performance(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return Py_BuildValue("b", mvApp::GetApp()->usingThreadPoolHighPerformance());
	}

	PyObject* get_threadpool_timeout(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return Py_BuildValue("f", mvApp::GetApp()->getThreadPoolTimeout());
	}

	PyObject* get_active_window(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return Py_BuildValue("s", mvApp::GetApp()->getActiveWindow());
	}

	PyObject* get_marvel_version(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		return Py_BuildValue("s", mvApp::GetApp()->GetVersion());
	}

	PyObject* set_threadpool_timeout(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float time;

		if (!Parsers["set_threadpool_timeout"].parse(args, kwargs, __FUNCTION__, &time))
			Py_RETURN_NONE;

		mvApp::GetApp()->setThreadPoolTimeout(time);

		Py_RETURN_NONE;
	}

	PyObject* set_thread_count(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		int threads;

		if (!Parsers["set_thread_count"].parse(args, kwargs, __FUNCTION__, &threads))
			Py_RETURN_NONE;

		mvApp::GetApp()->setThreadCount(threads);

		Py_RETURN_NONE;
	}

	PyObject* add_data(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* name;
		PyObject* data;

		if (!Parsers["add_data"].parse(args, kwargs, __FUNCTION__, &name, &data))
			Py_RETURN_NONE;

		mvApp::GetApp()->addData(name, data);
		Py_XINCREF(data);

		Py_RETURN_NONE;
	}

	PyObject* get_data(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* name;

		if (!Parsers["get_data"].parse(args, kwargs, __FUNCTION__, &name))
			Py_RETURN_NONE;

		auto result = mvApp::GetApp()->getData(name);

		if (result)
			return result;

		Py_RETURN_NONE;
	}

	PyObject* delete_data(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* name;

		if (!Parsers["delete_data"].parse(args, kwargs, __FUNCTION__, &name))
			Py_RETURN_NONE;

		mvApp::GetApp()->deleteData(name);

		Py_RETURN_NONE;
	}

	PyObject* set_threadpool_high_performance(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->setThreadPoolHighPerformance();
		Py_RETURN_NONE;
	}

	PyObject* add_item_color_style(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;
		int style;
		PyObject* color;

		if (!Parsers["add_item_color_style"].parse(args, kwargs, __FUNCTION__, &item, &style, &color))
			Py_RETURN_NONE;

		auto mcolor = mvPythonTranslator::getColor(color);

		mvApp::GetApp()->addItemColorStyle(item, style, mcolor);

		Py_RETURN_NONE;
	}

	PyObject* is_item_hovered(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["is_item_hovered"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->isItemHovered());

		Py_RETURN_NONE;
	}

	PyObject* is_item_active(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["is_item_active"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->isItemActive());

		Py_RETURN_NONE;
	}

	PyObject* is_item_focused(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["is_item_focused"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->isItemFocused());

		Py_RETURN_NONE;
	}

	PyObject* is_item_clicked(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["is_item_clicked"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->isItemClicked());

		Py_RETURN_NONE;
	}

	PyObject* is_item_visible(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["is_item_visible"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->isItemVisible());

		Py_RETURN_NONE;
	}

	PyObject* is_item_edited(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["is_item_edited"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->isItemEdited());

		Py_RETURN_NONE;
	}

	PyObject* is_item_activated(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["is_item_activated"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->isItemActivated());

		Py_RETURN_NONE;
	}

	PyObject* is_item_deactivated(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["is_item_deactivated"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->isItemDeactivated());

		Py_RETURN_NONE;
	}

	PyObject* is_item_deactivated_after_edit(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["is_item_deactivated_after_edit"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->isItemDeactivatedAfterEdit());

		Py_RETURN_NONE;
	}

	PyObject* is_item_toggled_open(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["is_item_toggled_open"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return Py_BuildValue("i", appitem->isItemToogledOpen());

		Py_RETURN_NONE;
	}

	PyObject* get_item_rect_min(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["get_item_rect_min"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
		{
			mvVec2 value = appitem->getItemRectMin();
			PyObject* pvalue = PyTuple_New(2);
			PyTuple_SetItem(pvalue, 0, PyFloat_FromDouble(value.x));
			PyTuple_SetItem(pvalue, 1, PyFloat_FromDouble(value.y));
			return pvalue;
		}

		Py_RETURN_NONE;
	}

	PyObject* get_item_rect_max(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["get_item_rect_max"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
		{
			mvVec2 value = appitem->getItemRectMax();
			PyObject* pvalue = PyTuple_New(2);
			PyTuple_SetItem(pvalue, 0, PyFloat_FromDouble(value.x));
			PyTuple_SetItem(pvalue, 1, PyFloat_FromDouble(value.y));
			return pvalue;
		}

		Py_RETURN_NONE;
	}

	PyObject* get_item_rect_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!Parsers["get_item_rect_size"].parse(args, kwargs,__FUNCTION__, &item))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
		{
			mvVec2 value = appitem->getItemRectSize();
			PyObject* pvalue = PyTuple_New(2);
			PyTuple_SetItem(pvalue, 0, PyFloat_FromDouble(value.x));
			PyTuple_SetItem(pvalue, 1, PyFloat_FromDouble(value.y));
			return pvalue;
		}

		Py_RETURN_NONE;
	}

	PyObject* change_style_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int item;
		float x = 0.0f;
		float y = 0.0f;

		if (!Parsers["change_style_item"].parse(args, kwargs,__FUNCTION__, &item, &x, &y))
			Py_RETURN_NONE;

		mvApp::GetApp()->changeStyleItem(item, x, y);

		Py_RETURN_NONE;
	}

	PyObject* change_theme_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int item;
		int r, g, b, a;

		if(!Parsers["change_theme_item"].parse(args, kwargs,__FUNCTION__, &item, &r, &g, &b, &a))
			Py_RETURN_NONE;

		mvApp::GetApp()->changeThemeItem(item, { r, g, b, a });

		

		Py_RETURN_NONE;
	}

	PyObject* get_value(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if(!Parsers["get_value"].parse(args, kwargs,__FUNCTION__, &name))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(std::string(name));

		if(item == nullptr)
			Py_RETURN_NONE;

		return item->getPyValue();
	}

	PyObject* set_value(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* value;

		if(!Parsers["set_value"].parse(args, kwargs,__FUNCTION__, &name, &value))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(std::string(name));

		if (item == nullptr)
			Py_RETURN_NONE;

		item->setPyValue(value);
		Py_RETURN_NONE;
	}

	PyObject* show_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if(!Parsers["show_item"].parse(args, kwargs,__FUNCTION__, &name))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(std::string(name));

		if (item != nullptr)
			item->show();

		Py_RETURN_NONE;
	}

	PyObject* hide_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if(!Parsers["hide_item"].parse(args, kwargs,__FUNCTION__, &name))
			Py_RETURN_NONE;

		mvAppItem* item = mvApp::GetApp()->getItem(std::string(name));

		if (item != nullptr)
			item->hide();

		Py_RETURN_NONE;
	}

	PyObject* set_main_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;


		if(!Parsers["set_main_callback"].parse(args, kwargs,__FUNCTION__, &callback))
			Py_RETURN_NONE;

		mvApp::GetApp()->setMainCallback(std::string(callback));

		

		Py_RETURN_NONE;
	}

	PyObject* set_item_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* callback;
		const char* item;
		
		if(!Parsers["set_item_callback"].parse(args, kwargs,__FUNCTION__, &item, &callback))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			appitem->setCallback(callback);

		Py_RETURN_NONE;
	}

	PyObject* set_item_popup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* popup;
		const char* item;

		if (!Parsers["set_item_popup"].parse(args, kwargs,__FUNCTION__, &item, &popup))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			appitem->setPopup(popup);

		Py_RETURN_NONE;
	}

	PyObject* set_item_tip(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* tip, * item;

		if(!Parsers["set_item_tip"].parse(args, kwargs,__FUNCTION__, &item, &tip))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			appitem->setTip(tip);

		Py_RETURN_NONE;
	}

	PyObject* set_item_width(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;
		int width;

		if(!Parsers["set_item_width"].parse(args, kwargs,__FUNCTION__, &item, &width))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			appitem->setWidth(width);
		

		Py_RETURN_NONE;
	}

	PyObject* set_item_height(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;
		int height;

		if (!Parsers["set_item_height"].parse(args, kwargs, __FUNCTION__, &item, &height))
			Py_RETURN_NONE;

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			appitem->setHeight(height);


		Py_RETURN_NONE;
	}

	PyObject* set_main_window_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int width;
		int height;

		if (!Parsers["set_main_window_size"].parse(args, kwargs, __FUNCTION__, &width, &height))
			Py_RETURN_NONE;

		mvApp::GetApp()->setWindowSize(width, height);

		Py_RETURN_NONE;
	}

	PyObject* close_popup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!mvApp::GetApp()->isStarted())
			Py_RETURN_NONE;

		ImGui::CloseCurrentPopup();
		
		Py_RETURN_NONE;
	}

	PyObject* set_theme(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* theme;

		if(!Parsers["set_theme"].parse(args, kwargs,__FUNCTION__, &theme))
			Py_RETURN_NONE;

		mvApp::GetApp()->setAppTheme(std::string(theme));

		Py_RETURN_NONE;
	}

	PyObject* show_metrics(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->showStandardWindow("metrics");
		Py_RETURN_NONE;
	}

	PyObject* show_about(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->showStandardWindow("about");
		Py_RETURN_NONE;
	}

	PyObject* show_source(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->showStandardWindow("source");
		Py_RETURN_NONE;
	}

	PyObject* show_debug(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->showStandardWindow("debug");
		Py_RETURN_NONE;
	}

	PyObject* show_documentation(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->showStandardWindow("documentation");
		Py_RETURN_NONE;
	}

	mvPythonModule* CreatePythonInterface()
	{

		mvPythonModule* pyModule = new mvPythonModule("sbApp", {});

		pyModule->addMethodD(add_column);
		pyModule->addMethodD(insert_column);
		pyModule->addMethodD(delete_column);
		pyModule->addMethodD(add_row);
		pyModule->addMethodD(insert_row);
		pyModule->addMethodD(delete_row);
		pyModule->addMethodD(get_table_item);
		pyModule->addMethodD(set_table_item);
		pyModule->addMethodD(set_table_selection);
		pyModule->addMethodD(get_table_selections);
		pyModule->addMethodD(get_delta_time);
		pyModule->addMethodD(get_total_time);
		pyModule->addMethodD(get_data);
		pyModule->addMethodD(delete_data);
		pyModule->addMethodD(add_data);
		pyModule->addMethodD(run_async_function);
		pyModule->addMethodD(save_file_dialog);
		pyModule->addMethodD(open_file_dialog);
		pyModule->addMethodD(delete_item);
		pyModule->addMethodD(move_item_down);
		pyModule->addMethodD(move_item_up);
		pyModule->addMethodD(get_style_item);
		pyModule->addMethodD(get_theme_item);
		pyModule->addMethodD(get_item_callback);
		pyModule->addMethodD(get_item_height);
		pyModule->addMethodD(get_item_popup);
		pyModule->addMethodD(get_item_tip);
		pyModule->addMethodD(get_main_callback);
		pyModule->addMethodD(get_main_window_size);
		pyModule->addMethodD(get_theme);
		pyModule->addMethodD(get_thread_count);
		pyModule->addMethodD(is_threadpool_high_performance);
		pyModule->addMethodD(get_threadpool_timeout);
		pyModule->addMethodD(get_active_window);
		pyModule->addMethodD(get_marvel_version);
		pyModule->addMethodD(show_source);
		pyModule->addMethodD(show_about);
		pyModule->addMethodD(show_debug);
		pyModule->addMethodD(show_metrics);
		pyModule->addMethodD(close_popup);
		pyModule->addMethodD(show_documentation);
		pyModule->addMethodD(set_threadpool_timeout);
		pyModule->addMethodD(set_thread_count);
		pyModule->addMethodD(set_threadpool_high_performance);
		pyModule->addMethodD(set_main_window_size);
		pyModule->addMethodD(add_item_color_style);
		pyModule->addMethodD(set_item_popup);
		pyModule->addMethodD(is_item_hovered);
		pyModule->addMethodD(is_item_active);
		pyModule->addMethodD(is_item_focused);
		pyModule->addMethodD(is_item_clicked);
		pyModule->addMethodD(is_item_visible);
		pyModule->addMethodD(is_item_edited);
		pyModule->addMethodD(is_item_activated);
		pyModule->addMethodD(is_item_deactivated);
		pyModule->addMethodD(is_item_deactivated_after_edit);
		pyModule->addMethodD(is_item_toggled_open);
		pyModule->addMethodD(get_item_rect_min);
		pyModule->addMethodD(get_item_rect_max);
		pyModule->addMethodD(get_item_rect_size);
		pyModule->addMethodD(change_style_item);
		pyModule->addMethodD(show_item);
		pyModule->addMethodD(hide_item);
		pyModule->addMethodD(change_theme_item);
		pyModule->addMethodD(set_theme);
		pyModule->addMethodD(set_main_callback);
		pyModule->addMethodD(set_item_callback);
		pyModule->addMethodD(set_item_tip);
		pyModule->addMethodD(set_item_width);
		pyModule->addMethodD(set_item_height);
		pyModule->addMethodD(get_value);
		pyModule->addMethodD(set_value);

		return pyModule;
	}
}