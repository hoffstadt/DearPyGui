#include "mvAppItemInterface.h"
#include "mvInterfaceCore.h"
#include <ImGuiFileDialog.h>

namespace Marvel {

	void AddItemCommands(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "get_item_configuration", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns an items configuration", "dict", "Widget Commands") });

		parsers->insert({ "configure_item", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::Object, "config"},
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

		parsers->insert({ "set_window_pos", mvPythonParser({
			{mvPythonDataType::String, "window"},
			{mvPythonDataType::Float, "x"},
			{mvPythonDataType::Float, "y"}
		}, "Sets a windows position", "None", "Widget Commands") });

		parsers->insert({ "get_window_pos", mvPythonParser({
			{mvPythonDataType::String, "window"}
		}, "Gets a windows position", "List[float]", "Widget Commands") });

		parsers->insert({ "delete_item", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Bool, "children_only"}
		}, "Deletes an item if it exists.", "None", "Widget Commands") });

		parsers->insert({ "does_item_exist", mvPythonParser({
		{mvPythonDataType::String, "item"},
		}, "Checks if item exists.", "Bool", "Widget Commands") });

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

		parsers->insert({ "get_item_popup", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns an item's popup.", "str", "Widget Commands") });

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
		}, "Sets an item's value if applicable.", "None", "Widget Commands") });

		parsers->insert({ "show_item", mvPythonParser({
			{mvPythonDataType::String, "name"}
		}, "Shows an item if it was hidden.", "None", "Widget Commands") });

		parsers->insert({ "hide_item", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Bool, "children_only"}
		}, "Hides an item.", "None", "Widget Commands") });

		parsers->insert({ "set_item_callback", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
		}, "Sets an item's callback if applicable.", "None", "Widget Commands") });

		parsers->insert({ "set_item_popup", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::String, "popup"}
		}, "Sets an item's popup if applicable.", "None", "Widget Commands") });

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

	PyObject* get_item_configuration(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["get_item_configuration"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
		{
			PyObject* pdict = PyDict_New();
			appitem->getConfigDict(pdict);
			appitem->getExtraConfigDict(pdict);
			return pdict;
		}

		return GetPyNone();
	}

	PyObject* configure_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvGlobalIntepreterLock gil;

		std::string item = ToString(PyTuple_GetItem(args, 0));

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
		{
			appitem->setConfigDict(kwargs);
			appitem->setExtraConfigDict(kwargs);
		}

		return GetPyNone();
	}

	PyObject* set_window_pos(PyObject * self, PyObject * args, PyObject * kwargs)
	{
		const char* window;
		float x;
		float y;

		if (!(*mvApp::GetApp()->getParsers())["set_window_pos"].parse(args, kwargs, __FUNCTION__, &window, &x, &y))
			return GetPyNone();

		if (std::string(window) == "logger##standard")
		{
			mvAppLog::SetWindowPos(x, y);
			return GetPyNone();
		}

		// check if item is a regular item
		mvWindowAppitem* awindow = mvApp::GetApp()->getWindow(window);

		// check if item is a standard window
		mvStandardWindow* swindow = nullptr;
		if (awindow == nullptr)
			swindow = mvApp::GetApp()->getStandardWindow(window);
		else
		{
			awindow->setWindowPos(x, y);
			return GetPyNone();
		}

		if (swindow == nullptr)
		{
			ThrowPythonException(window + std::string(" window was not found"));
			return GetPyNone();
		}
		else
		{
			swindow->setWindowPos(x, y);
			return GetPyNone();
		}
	}

	PyObject* get_window_pos(PyObject * self, PyObject * args, PyObject * kwargs)
	{
		const char* window;

		if (!(*mvApp::GetApp()->getParsers())["get_window_pos"].parse(args, kwargs, __FUNCTION__, &window))
			return GetPyNone();

		if (std::string(window) == "logger##standard")
			return ToPyPair(mvAppLog::GetWindowPos().x, mvAppLog::GetWindowPos().y);

		// check if item is a regular item
		mvWindowAppitem* awindow = mvApp::GetApp()->getWindow(window);

		// check if item is a standard window
		mvStandardWindow* swindow = nullptr;
		if (awindow == nullptr)
			swindow = mvApp::GetApp()->getStandardWindow(window);
		else
			return ToPyPair(awindow->getWindowPos().x, awindow->getWindowPos().y);

		if (swindow == nullptr)
		{
			ThrowPythonException(window + std::string(" window was not found"));
			return GetPyNone();
		}
		else
			return ToPyPair(swindow->getWindowPos().x, swindow->getWindowPos().y);

	}

	PyObject* delete_item(PyObject * self, PyObject * args, PyObject * kwargs)
	{
		if (!mvApp::GetApp()->checkIfMainThread())
			return GetPyNone();

		const char* item;
		int childrenOnly = false;

		if (!(*mvApp::GetApp()->getParsers())["delete_item"].parse(args, kwargs, __FUNCTION__, &item, &childrenOnly))
			return GetPyNone();

		if (childrenOnly)
			mvApp::GetApp()->deleteItemChildren(item);
		else
			mvApp::GetApp()->deleteItem(item);

		return GetPyNone();

	}

	PyObject* does_item_exist(PyObject * self, PyObject * args, PyObject * kwargs)
	{
		if (!mvApp::GetApp()->checkIfMainThread())
			return GetPyNone();

		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["does_item_exist"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		if (!mvApp::GetApp()->getItem(item))
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

		mvApp::GetApp()->moveItemUp(item);

		return GetPyNone();

	}

	PyObject* move_item_down(PyObject * self, PyObject * args, PyObject * kwargs)
	{
		if (!mvApp::GetApp()->checkIfMainThread())
			return GetPyNone();

		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["move_item_down"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		mvApp::GetApp()->moveItemDown(item);

		return GetPyNone();

	}

	PyObject* get_item_callback(PyObject * self, PyObject * args, PyObject * kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["get_item_callback"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

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

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
		{
			auto children = appitem->getChildren();
			std::vector<std::string> childList;
			for (auto child : children)
				childList.emplace_back(child->getName());

			return ToPyList(childList);
		}

		return GetPyNone();
	}

	PyObject* get_all_items(PyObject * self, PyObject * args, PyObject * kwargs)
	{

		std::vector<mvAppItem*>& windows = mvApp::GetApp()->getWindows();

		std::vector<std::string> childList;

		// to help recursively retrieve children
		std::function<void(mvAppItem*)> ChildRetriever;
		ChildRetriever = [&childList, &ChildRetriever](mvAppItem* item) {
			auto children = item->getChildren();
			for (mvAppItem* child : children)
			{
				childList.emplace_back(child->getName());
				if (child->isContainer())
					ChildRetriever(child);
			}

		};

		for (auto window : windows)
			ChildRetriever(window);

		return ToPyList(childList);
	}

	PyObject* get_windows(PyObject * self, PyObject * args, PyObject * kwargs)
	{
		auto windows = mvApp::GetApp()->getWindows();
		std::vector<std::string> childList;
		for (auto window : windows)
			childList.emplace_back(window->getName());
		return ToPyList(childList);
	}

	PyObject* get_item_parent(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["get_item_parent"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return ToPyString(appitem->getParent()->getName());

		return GetPyNone();
	}

	PyObject* is_item_hovered(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_hovered"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return ToPyBool(appitem->isItemHovered());

		return GetPyNone();
	}

	PyObject* is_item_shown(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_shown"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return ToPyBool(appitem->isShown());

		return GetPyNone();
	}

	PyObject* is_item_active(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_active"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return ToPyBool(appitem->isItemActive());

		return GetPyNone();
	}

	PyObject* is_item_focused(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_focused"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return ToPyBool(appitem->isItemFocused());

		return GetPyNone();
	}

	PyObject* is_item_clicked(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_clicked"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return ToPyBool(appitem->isItemClicked());

		return GetPyNone();
	}

	PyObject* is_item_container(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_container"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return ToPyBool(appitem->isContainer());

		return GetPyNone();
	}

	PyObject* is_item_visible(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_visible"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return ToPyBool(appitem->isItemVisible());

		return GetPyNone();
	}

	PyObject* is_item_edited(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_edited"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return ToPyBool(appitem->isItemEdited());

		return GetPyNone();
	}

	PyObject* is_item_activated(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_activated"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return ToPyBool(appitem->isItemActivated());

		return GetPyNone();
	}

	PyObject* is_item_deactivated(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_deactivated"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return ToPyBool(appitem->isItemDeactivated());

		return GetPyNone();
	}

	PyObject* is_item_deactivated_after_edit(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_deactivated_after_edit"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return ToPyBool(appitem->isItemDeactivatedAfterEdit());

		return GetPyNone();
	}

	PyObject* is_item_toggled_open(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["is_item_toggled_open"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
			return ToPyBool(appitem->isItemToogledOpen());

		return GetPyNone();
	}

	PyObject* get_item_rect_min(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["get_item_rect_min"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
		{
			mvVec2 value = appitem->getItemRectMin();
			return ToPyPair(value.x, value.y);
		}

		return GetPyNone();
	}

	PyObject* get_item_rect_max(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["get_item_rect_max"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
		{
			mvVec2 value = appitem->getItemRectMax();
			return ToPyPair(value.x, value.y);
		}

		return GetPyNone();
	}

	PyObject* get_item_rect_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["get_item_rect_size"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		auto appitem = mvApp::GetApp()->getItem(item);

		if (appitem)
		{
			mvVec2 value = appitem->getItemRectSize();
			return ToPyPair(value.x, value.y);
		}

		return GetPyNone();
	}

	PyObject* get_value(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if (!(*mvApp::GetApp()->getParsers())["get_value"].parse(args, kwargs, __FUNCTION__, &name))
			return GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(std::string(name));

		if (item == nullptr)
			return GetPyNone();

		// if compile time, check for data source usage
		if (!mvApp::IsAppStarted())
		{
			std::string datasource = item->getDataSource();
			if (!datasource.empty())
				return mvDataStorage::GetData(datasource);
		}

		return item->getPyValue();
	}

	PyObject* set_value(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* value;

		if (!(*mvApp::GetApp()->getParsers())["set_value"].parse(args, kwargs, __FUNCTION__, &name, &value))
			return GetPyNone();

		mvAppItem* item = mvApp::GetApp()->getItem(std::string(name));

		if (item == nullptr)
			return GetPyNone();

		if (item->getDataSource().empty())
			item->setPyValue(value);
		else
			mvDataStorage::AddData(item->getDataSource(), value);

		return GetPyNone();
	}

	PyObject* set_item_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;
		PyObject* callback_data = nullptr;
		const char* item;

		if (!(*mvApp::GetApp()->getParsers())["set_item_callback"].parse(args, kwargs, __FUNCTION__, &item, &callback, &callback_data))
			return GetPyNone();

		mvAppItem* appitem;
		appitem = mvApp::GetApp()->getItem(item);

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
}