#include "mvAppItemInterface.h"
#include "mvEvents.h"
#include <ImGuiFileDialog.h>
#include "mvValueStorage.h"

namespace Marvel {

	void AddItemCommands(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "get_managed_column_width", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::Integer, "column"},
		}, "Returns the width of the ith column.", "Float", "Widget Commands") });

		parsers->insert({ "set_managed_column_width", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::Integer, "column"},
			{mvPythonDataType::Float, "width"},
		}, "Sets the width of the ith column.", "None", "Widget Commands") });

		parsers->insert({ "get_item_callback_data", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns an item' callback data", "Any", "Widget Commands") });

		parsers->insert({ "set_item_callback_data", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
		}, "Sets an item's callback data if applicable.", "None", "Widget Commands") });

		parsers->insert({ "move_item", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "parent", "", "''"},
			{mvPythonDataType::String, "before", "", "''"}
		}, "Moves an existing item.", "None", "Widget Commands") });

		parsers->insert({ "get_item_type", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns an item's type", "str", "Widget Commands") });

		parsers->insert({ "get_item_configuration", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns an items configuration", "dict", "Widget Commands") });

		parsers->insert({ "configure_item", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::Kwargs, "**Kwargs"},
		}, "Configures an item", "None", "Widget Commands") });

		parsers->insert({ "get_windows", mvPythonParser({
		}, "Returns a list of windows.", "List[str]", "Widget Commands") });

		parsers->insert({ "get_all_items", mvPythonParser({
		}, "Returns a list of all items.", "List[str]", "Widget Commands") });

		parsers->insert({ "get_item_children", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns a list of an item's children.", "List[str]", "Widget Commands") });
		
		parsers->insert({ "set_item_label", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::String, "label"},
		}, "Sets an item's label.", "None", "Widget Commands") });

		parsers->insert({ "get_item_label", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Gets an item's label.", "str", "Widget Commands") });

		parsers->insert({ "delete_item", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "children_only", "delete children only", "False"}
		}, "Deletes an item if it exists.", "None", "Widget Commands") });

		parsers->insert({ "does_item_exist", mvPythonParser({
		{mvPythonDataType::String, "item"},
		}, "Checks if item exists.", "bool", "Widget Commands") });

		parsers->insert({ "move_item_up", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Moves item up if possible and if it exists.", "None", "Widget Commands") });

		parsers->insert({ "move_item_down", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Moves item down if possible and if it exists.", "None", "Widget Commands") });

		parsers->insert({ "get_item_callback", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns an item' callback", "Callable", "Widget Commands") });

		parsers->insert({ "get_item_height", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns an item's height.", "float", "Widget Commands") });

		parsers->insert({ "get_item_width", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns an item's width.", "float", "Widget Commands") });

		parsers->insert({ "get_item_parent", mvPythonParser({
		{mvPythonDataType::String, "item"}
		}, "Returns an item's parent.", "str", "Widget Commands") });

		parsers->insert({ "get_item_tip", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns an item's tip.", "str", "Widget Commands") });

		parsers->insert({ "is_item_hovered", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item is hovered.", "bool", "Widget Commands") });

		parsers->insert({ "is_item_shown", mvPythonParser({
		{mvPythonDataType::String, "item"},
		}, "Checks if an item is shown.", "bool", "Widget Commands") });

		parsers->insert({ "is_item_active", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item is active.", "bool", "Widget Commands") });

		parsers->insert({ "is_item_focused", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item is focused.", "bool", "Widget Commands") });

		parsers->insert({ "is_item_clicked", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item is clicked.", "bool", "Widget Commands") });

		parsers->insert({ "is_item_container", mvPythonParser({
		{mvPythonDataType::String, "item"},
		}, "Checks if an item is container.", "bool", "Widget Commands") });

		parsers->insert({ "is_item_visible", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item is visible on screen.", "bool", "Widget Commands") });

		parsers->insert({ "is_item_edited", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item has been edited.", "bool", "Widget Commands") });

		parsers->insert({ "is_item_activated", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item has been activated.", "bool", "Widget Commands") });

		parsers->insert({ "is_item_deactivated", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item has been deactivated.", "bool", "Widget Commands") });

		parsers->insert({ "is_item_deactivated_after_edit", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item has been deactivated after editing.", "bool", "Widget Commands") });

		parsers->insert({ "is_item_toggled_open", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item is toggled.", "bool", "Widget Commands") });

		parsers->insert({ "get_item_rect_min", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Returns an item's minimum allowable size. [width, height]", "[float, float]", "Widget Commands") });

		parsers->insert({ "get_item_rect_max", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Returns an item's maximum allowable size. [width, height]", "[float, float]", "Widget Commands") });

		parsers->insert({ "get_item_rect_size", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Returns an item's current size. [width, height]", "[float, float]", "Widget Commands") });

		parsers->insert({ "get_value", mvPythonParser({
			{mvPythonDataType::String, "name"}
		}, "Returns an item's value or None if there is none.", "Any", "Widget Commands") });

		parsers->insert({ "set_value", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Object, "value"}
		}, "Sets an item's value if applicable.", "bool", "Widget Commands") });

		parsers->insert({ "add_value", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Object, "value"}
		}, "Adds a value to the value storage.", "None", "Widget Commands") });

		parsers->insert({ "incref_value", mvPythonParser({
			{mvPythonDataType::String, "name"}
		}, "Increases the reference count of a value.", "None", "Widget Commands") });

		parsers->insert({ "decref_value", mvPythonParser({
			{mvPythonDataType::String, "name"}
		}, "Decreases the reference count of a value.", "None", "Widget Commands") });

		parsers->insert({ "show_item", mvPythonParser({
			{mvPythonDataType::String, "name"}
		}, "Shows an item if it was hidden.", "None", "Widget Commands") });

		parsers->insert({ "set_item_callback", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::Callable, "callback", "Registers a callback"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
		}, "Sets an item's callback if applicable.", "None", "Widget Commands") });

		parsers->insert({ "set_item_tip", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::String, "tip"}
		}, "Sets a simple tooltip for an item.", "None", "Widget Commands") });

		parsers->insert({ "set_item_width", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::Integer, "width"}
		}, "Sets an item's width.", "None", "Widget Commands") });

		parsers->insert({ "set_item_height", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::Integer, "height"}
		}, "Sets an item's height if applicable.", "None", "Widget Commands") });
	}

	PyObject* get_managed_column_width(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* managed_columns;
		int column;

		if (!(*mvApp::GetApp()->getParsers())["get_managed_column_width"].parse(args, kwargs, __FUNCTION__,
			&managed_columns, &column))
			return ToPyFloat(0.0f);

		auto item = mvApp::GetApp()->getItemRegistry().getItem(managed_columns);

		if (item == nullptr)
		{
			std::string message = managed_columns;
			ThrowPythonException(message + " managed_columns does not exist.");
			return ToPyFloat(0.0f);
		}

		mvManagedColumns* columns;
		if (item->getType() == mvAppItemType::ManagedColumns)
			columns = static_cast<mvManagedColumns*>(item.get());
		else
		{
			ThrowPythonException(std::string(managed_columns) + " is not a managed columns.");
			return ToPyFloat(0.0f);
		}

		return ToPyFloat(columns->getColumnWidth(column));
	}

	PyObject* set_managed_column_width(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* managed_columns;
		int column;
		float width;

		if (!(*mvApp::GetApp()->getParsers())["set_managed_column_width"].parse(args, kwargs, __FUNCTION__,
			&managed_columns, &column, &width))
			return GetPyNone();

		auto item = mvApp::GetApp()->getItemRegistry().getItem(managed_columns);

		if (item == nullptr)
		{
			std::string message = managed_columns;
			ThrowPythonException(message + " managed_columns does not exist.");
			return GetPyNone();
		}

		mvManagedColumns* columns;
		if (item->getType() == mvAppItemType::ManagedColumns)
		{
			columns = static_cast<mvManagedColumns*>(item.get());
			columns->setColumnWidth(column, width);
		}
		else
			ThrowPythonException(std::string(managed_columns) + " is not a managed columns.");

		return GetPyNone();
	}

	PyObject* move_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* item;
		const char* parent = "";
		const char* before = "";

		if (!(*mvApp::GetApp()->getParsers())["move_item"].parse(args, kwargs, __FUNCTION__, 
			&item, &parent, &before))
			return GetPyNone();

		mvApp::GetApp()->getCallbackRegistry().submit([item, parent, before]()
			{mvApp::GetApp()->getItemRegistry().moveItem(item, parent, before); });

		return GetPyNone();
	}

	PyObject* get_item_configuration(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["get_item_configuration"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		if (std::string(item) == "logger##standard")
		{
			PyObject* pdict = PyDict_New();
			mvAppLog::GetConfigDict(pdict);
			return pdict;
		}

		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
		{
			PyObject* pdict = PyDict_New();
			appitem->getConfigDict(pdict);
			appitem->getExtraConfigDict(pdict);
			return pdict;
		}
		else
			ThrowPythonException(item + std::string(" item was not found"));

		return GetPyNone();
	}

	PyObject* configure_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		 

		std::string item = ToString(PyTuple_GetItem(args, 0));

		if (std::string(item) == "logger##standard")
		{
			mvAppLog::SetConfigDict(kwargs);
			return GetPyNone();
		}

		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
		{
			appitem->checkConfigDict(kwargs);
			appitem->setConfigDict(kwargs);
			appitem->setExtraConfigDict(kwargs);
		}
		else
			ThrowPythonException(item + std::string(" item was not found"));

		return GetPyNone();
	}

	PyObject* delete_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!mvApp::GetApp()->checkIfMainThread())
			return GetPyNone();

		const char* item;
		int childrenOnly = false;

		if (!(*mvApp::GetApp()->getParsers())["delete_item"].parse(args, kwargs, __FUNCTION__, &item, &childrenOnly))
			return GetPyNone();

		mvApp::GetApp()->getCallbackRegistry().submit([item, childrenOnly]() 
			{mvApp::GetApp()->getItemRegistry().deleteItem(item, childrenOnly); });

		return GetPyNone();

	}

	PyObject* does_item_exist(PyObject * self, PyObject * args, PyObject * kwargs)
	{
		if (!mvApp::GetApp()->checkIfMainThread())
			return GetPyNone();

		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["does_item_exist"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		if (!mvApp::GetApp()->getItemRegistry().getItem(item))
			return ToPyBool(false);

		return ToPyBool(true);

	}

	PyObject* move_item_up(PyObject * self, PyObject * args, PyObject * kwargs)
	{
		if (!mvApp::GetApp()->checkIfMainThread())
			return GetPyNone();

		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["move_item_up"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		mvApp::GetApp()->getCallbackRegistry().submit([item]()
			{mvApp::GetApp()->getItemRegistry().moveItemUp(item); });

		return GetPyNone();

	}

	PyObject* move_item_down(PyObject * self, PyObject * args, PyObject * kwargs)
	{
		if (!mvApp::GetApp()->checkIfMainThread())
			return GetPyNone();

		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["move_item_down"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		mvApp::GetApp()->getCallbackRegistry().submit([item]()
			{mvApp::GetApp()->getItemRegistry().moveItemUp(item); });

		return GetPyNone();
	}

	PyObject* get_item_callback(PyObject * self, PyObject * args, PyObject * kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["get_item_callback"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
		{
			PyObject* callback = appitem->getCallback();
			if (callback)
			{
				Py_XINCREF(callback);
				return callback;
			}

		}

		return GetPyNone();
	}

	PyObject* get_item_children(PyObject * self, PyObject * args, PyObject * kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["get_item_children"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto childlist = mvApp::GetApp()->getItemRegistry().getItemChildren(item);

		if (!childlist.empty())
			return ToPyList(childlist);

		return GetPyNone();
	}

	PyObject* get_all_items(PyObject * self, PyObject * args, PyObject * kwargs)
	{

		auto childlist = mvApp::GetApp()->getItemRegistry().getAllItems();

		return ToPyList(childlist);
	}

	PyObject* get_windows(PyObject * self, PyObject * args, PyObject * kwargs)
	{
		auto windowlist = mvApp::GetApp()->getItemRegistry().getWindows();

		return ToPyList(windowlist);
	}

	PyObject* get_item_parent(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["get_item_parent"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto parent = mvApp::GetApp()->getItemRegistry().getItemParentName(item);

		if (!parent.empty())
			return ToPyString(parent);

		return GetPyNone();
	}

	PyObject* is_item_hovered(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_hovered"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
			return ToPyBool(appitem->getState().isItemHovered());

		return GetPyNone();
	}

	PyObject* is_item_shown(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_shown"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
			return ToPyBool(appitem->isShown());

		return GetPyNone();
	}

	PyObject* is_item_active(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_active"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
			return ToPyBool(appitem->getState().isItemActive());

		return GetPyNone();
	}

	PyObject* is_item_focused(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_focused"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
			return ToPyBool(appitem->getState().isItemFocused());

		return GetPyNone();
	}

	PyObject* is_item_clicked(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_clicked"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
			return ToPyBool(appitem->getState().isItemClicked());

		return GetPyNone();
	}

	PyObject* is_item_container(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_container"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
			return ToPyBool(appitem->getDescription().container);

		return GetPyNone();
	}

	PyObject* is_item_visible(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_visible"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
			return ToPyBool(appitem->getState().isItemVisible());

		return GetPyNone();
	}

	PyObject* is_item_edited(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_edited"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
			return ToPyBool(appitem->getState().isItemEdited());

		return GetPyNone();
	}

	PyObject* is_item_activated(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_activated"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
			return ToPyBool(appitem->getState().isItemActivated());

		return GetPyNone();
	}

	PyObject* is_item_deactivated(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_deactivated"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
			return ToPyBool(appitem->getState().isItemDeactivated());

		return GetPyNone();
	}

	PyObject* is_item_deactivated_after_edit(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_deactivated_after_edit"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
			return ToPyBool(appitem->getState().isItemDeactivatedAfterEdit());

		return GetPyNone();
	}

	PyObject* is_item_toggled_open(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_toggled_open"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
			return ToPyBool(appitem->getState().isItemToogledOpen());

		return GetPyNone();
	}

	PyObject* get_item_rect_min(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["get_item_rect_min"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
		{
			mvVec2 value = appitem->getState().getItemRectMin();
			return ToPyPair(value.x, value.y);
		}

		return GetPyNone();
	}

	PyObject* get_item_rect_max(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["get_item_rect_max"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
		{
			mvVec2 value = appitem->getState().getItemRectMax();
			return ToPyPair(value.x, value.y);
		}

		return GetPyNone();
	}

	PyObject* get_item_rect_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["get_item_rect_size"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
		{
			mvVec2 value = appitem->getState().getItemRectSize();
			return ToPyPair(value.x, value.y);
		}

		return GetPyNone();
	}

	PyObject* get_value(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if (!(*mvApp::GetApp()->getParsers())["get_value"].parse(args, kwargs, __FUNCTION__, &name))
			return GetPyNone();

		return mvApp::GetApp()->getValueStorage().GetPyValue(name);
	}

	PyObject* set_value(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* value;

		if (!(*mvApp::GetApp()->getParsers())["set_value"].parse(args, kwargs, __FUNCTION__, &name, &value))
			return GetPyNone();

		if (value)
			Py_XINCREF(value);
		
		mvEventBus::PublishEndFrame(mvEVT_CATEGORY_VALUES, mvEVT_PY_SET_VALUE, {
			CreateEventArgument("NAME", std::string(name)),
			CreateEventPtrArgument("VALUE", value)
			});

		return GetPyNone();
		//return ToPyBool(mvApp::GetApp()->getValueStorage().SetPyValue(name, value));
	}

	PyObject* add_value(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* value;

		if (!(*mvApp::GetApp()->getParsers())["add_value"].parse(args, kwargs, __FUNCTION__, &name, &value))
			return GetPyNone();

		mvApp::GetApp()->getValueStorage().AddPyValue(name, value);
		return GetPyNone();
	}

	PyObject* incref_value(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if (!(*mvApp::GetApp()->getParsers())["incref_value"].parse(args, kwargs, __FUNCTION__, &name))
			return GetPyNone();

		mvApp::GetApp()->getValueStorage().IncrementRef(name);
		return GetPyNone();
	}

	PyObject* decref_value(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if (!(*mvApp::GetApp()->getParsers())["decref_value"].parse(args, kwargs, __FUNCTION__, &name))
			return GetPyNone();

		mvApp::GetApp()->getValueStorage().DecrementRef(name);
		return GetPyNone();
	}

	PyObject* set_item_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;
		PyObject* callback_data = nullptr;
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["set_item_callback"].parse(args, kwargs, __FUNCTION__, &item, &callback, &callback_data))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
		{
			if (appitem->getCallback() != callback)
				Py_XINCREF(callback);
			appitem->setCallback(callback);

			if (callback_data)
			{
				if (appitem->getCallbackData() != callback_data)
					Py_XINCREF(callback_data);
				appitem->setCallbackData(callback_data);
			}
		}

		return GetPyNone();
	}

	PyObject* get_item_type(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if (!(*mvApp::GetApp()->getParsers())["get_item_type"].parse(args, kwargs, __FUNCTION__, &name))
			return GetPyNone();

		auto item = mvApp::GetApp()->getItemRegistry().getItem(std::string(name));

		if (item == nullptr)
			return GetPyNone();

		return ToPyString(item->getStringType());
	}

	PyObject* get_item_callback_data(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["get_item_callback_data"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
		{
			PyObject* callback = appitem->getCallbackData();
			if (callback)
			{
				Py_XINCREF(callback);
				return callback;
			}

		}

		return GetPyNone();
	}

	PyObject* set_item_callback_data(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* data;
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["set_item_callback_data"].parse(args, kwargs, __FUNCTION__, &item, &data))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
		{
			if (appitem->getCallbackData() != data)
				Py_XINCREF(data);
			appitem->setCallbackData(data);
		}

		return GetPyNone();
	}
}