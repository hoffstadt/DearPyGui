#include "mvContainerInterface.h"
#include "mvInterfaceCore.h"

namespace Marvel {

	void AddContainerWidgets(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_menu_bar", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "show"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
		}, "Adds a menu bar to a window. Must be followed by a call to end_menu_bar.", "None", "Containers") });

		parsers->insert({ "add_menu", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "label"},
			{mvPythonDataType::Bool, "show"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::Bool, "enabled"},
		}, "Adds a menu to an existing menu bar. Must be followed by a call to end_menu.", "None", "Containers") });

		parsers->insert({ "add_menu_item", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "shortcut", "Adds a shortcut"},
			{mvPythonDataType::Bool, "check", "Makes menu with checkmarks."},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::String, "label"},
			{mvPythonDataType::Bool, "show"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
		}, "Adds a menu item to an existing menu.", "None", "Containers") });

		parsers->insert({ "add_tab_bar", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "reorderable", "allows for moveable tabs"},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
			{mvPythonDataType::Bool, "show"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "source", "data source for shared data"},
		}, "Adds a tab bar.", "None", "Containers") });

		parsers->insert({ "add_tab", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "closable", "creates a button on the tab that can hide the tab"},
			{mvPythonDataType::String, "label"},
			{mvPythonDataType::Bool, "show"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
		}, "Adds a tab to a tab bar. Must be closed with the end_tab command.", "None", "Containers") });

		parsers->insert({ "add_collapsing_header", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "default_open"},
			{mvPythonDataType::Bool, "closable"},
			{mvPythonDataType::String, "label"},
			{mvPythonDataType::Bool, "show"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"}
		}, "Adds a collapsing header to add items to. Must be closed with the end_collapsing_header command.",
			"None", "Containers") });

		parsers->insert({ "add_tree_node", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "default_open"},
			{mvPythonDataType::String, "label"},
			{mvPythonDataType::Bool, "show"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
		}, "Adds a tree node to add items to. Must be closed with the end_tree_node command.",
		"None", "Containers") });

		parsers->insert({ "add_group", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "show"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "horizontal"},
			{mvPythonDataType::Float, "horizontal_spacing",""},
			{mvPythonDataType::String, "popup"},
		}, "Creates a group that other widgets can belong to. The group allows item commands to be issued for all of its members.\
				Must be closed with the end_group command."
		, "None", "Containers") });

		parsers->insert({ "add_child", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "show"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height", ""},
			{mvPythonDataType::Bool, "border", ""},
			{mvPythonDataType::String, "popup", ""},
		}, "Adds an embedded child window. Will show scrollbars when items do not fit. Must be followed by a call to end_child.",
		"None", "Containers") });

		parsers->insert({ "add_window", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "width"},
			{mvPythonDataType::Integer, "height"},
			{mvPythonDataType::Integer, "x_pos", "x position the window will start at"},
			{mvPythonDataType::Integer, "y_pos", "y position the window will start at"},
			{mvPythonDataType::Bool, "autosize", "Autosized the window to fit it's items."},
			{mvPythonDataType::Bool, "no_resize", "Allows for the window size to be changed or fixed"},
			{mvPythonDataType::Bool, "no_title_bar", "Title name for the title bar of the window"},
			{mvPythonDataType::Bool, "no_move", "Allows for the window's position to be changed or fixed"},
			{mvPythonDataType::String, "label"},
			{mvPythonDataType::Bool, "show", "sets if the item is shown or not window."},
			{mvPythonDataType::Object, "on_close", "Callback ran when window is closed"},
		}, "Creates a new window for following items to be added to.",
			"None", "Containers") });

		parsers->insert({ "add_tooltip", mvPythonParser({
			{mvPythonDataType::String, "tipparent", "Sets the item's tool tip to be the same as the named item's tool tip"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::Bool, "show",""},
		}, "Adds an advanced tool tip for an item. This command must come immediately after the item the tip is for.",
			"None", "Containers") });

		parsers->insert({ "add_popup", mvPythonParser({
			{mvPythonDataType::String, "popupparent", "Parent that the popup will be assigned to."},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "mousebutton", "The mouse code that will trigger the popup. Default is 1 or mvMouseButton_Right. (mvMouseButton_Left, mvMouseButton_Right, mvMouseButton_Middle, mvMouseButton_X1, mvMouseButton_X2"},
			{mvPythonDataType::Bool, "modal"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height", ""},
			{mvPythonDataType::Bool, "show", ""},
		}, "Adds a popup window for an item. This command must come immediately after the item the popup is for. Must be followed by a call to end_popup.",
		"None", "Containers") });

		parsers->insert({ "end", mvPythonParser({
		}, "Ends a container.", "None", "Containers") });

	}

	PyObject* add_menu_bar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int show = true;
		const char* parent = "";
		const char* before = "";

		if (!(*mvApp::GetApp()->getParsers())["add_menu_bar"].parse(args, kwargs, __FUNCTION__, &name,
			&show, &parent, &before))
			return ToPyBool(false);

		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem->getType() == mvAppItemType::Window)
		{
			auto window = static_cast<mvWindowAppitem*>(parentItem);
			window->addFlag(ImGuiWindowFlags_MenuBar);

			mvAppItem* item = new mvMenuBar(name);

			item->setConfigDict(kwargs);
			item->setConfigDict(kwargs);
			item->setExtraConfigDict(kwargs);

			if (AddItemWithRuntimeChecks(item, parent, before))
			{
				mvApp::GetApp()->pushParent(item);
				return ToPyBool(true);
			}
		}

		else if (parentItem->getType() == mvAppItemType::Child)
		{
			auto child = static_cast<mvChild*>(parentItem);
			child->addFlag(ImGuiWindowFlags_MenuBar);

			mvAppItem* item = new mvMenuBar(name);

			item->setConfigDict(kwargs);
			item->setConfigDict(kwargs);
			item->setExtraConfigDict(kwargs);

			if (AddItemWithRuntimeChecks(item, parent, before))
			{
				mvApp::GetApp()->pushParent(item);
				return ToPyBool(true);
			}
		}

		return ToPyBool(false);
	}

	PyObject* add_menu(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* label = "";
		int show = true;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		int enabled = true;

		if (!(*mvApp::GetApp()->getParsers())["add_menu"].parse(args, kwargs, __FUNCTION__, &name,
			&label, &show, &tip, &parent, &before, &enabled))
			return ToPyBool(false);

		mvAppItem* item = new mvMenu(name);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		if (AddItemWithRuntimeChecks(item, parent, before))
		{
			mvApp::GetApp()->pushParent(item);
			return ToPyBool(true);
		}
		return ToPyBool(false);
	}

	PyObject* add_menu_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* shortcut = "";
		int check = false;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* label = "";
		int show = true;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";

		if (!(*mvApp::GetApp()->getParsers())["add_menu_item"].parse(args, kwargs, __FUNCTION__, &name,
			&shortcut, &check, &callback, &callback_data, &label, &show, &tip, &parent, &before))
			return ToPyBool(false);

		mvAppItem* item = new mvMenuItem(name);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_tab_bar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int reorderable = false;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		int show = true;
		const char* parent = "";
		const char* before = "";

		if (!(*mvApp::GetApp()->getParsers())["add_tab_bar"].parse(args, kwargs, __FUNCTION__, &name, &reorderable,
			&callback, &callback_data, &show, &parent, &before))
			return ToPyBool(false);

		mvAppItem* item = new mvTabBar(name);

		if (callback)
			Py_XINCREF(callback);

		item->setCallback(callback);

		if (callback_data)
			Py_XINCREF(callback_data);

		item->setCallbackData(callback_data);
		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		if (AddItemWithRuntimeChecks(item, parent, before))
		{
			mvApp::GetApp()->pushParent(item);
			return ToPyBool(true);
		}

		return ToPyBool(false);
	}

	PyObject* add_tab(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int closeable = false;
		const char* label = "";
		int show = true;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";

		if (!(*mvApp::GetApp()->getParsers())["add_tab"].parse(args, kwargs, __FUNCTION__, &name, &closeable,
			&label, &show, &tip, &parent, &before))
			return ToPyBool(false);

		if (std::string(parent).empty())
		{
			auto parentItem = mvApp::GetApp()->topParent();

			if (parentItem == nullptr)
			{
				ThrowPythonException("add_tab must follow a call to add_tabbar.");
				return ToPyBool(false);
			}

			else if (parentItem->getType() == mvAppItemType::TabBar)
			{
				mvAppItem* item = new mvTab(name);
				item->checkConfigDict(kwargs);
				item->setConfigDict(kwargs);
				item->setExtraConfigDict(kwargs);
				if (AddItemWithRuntimeChecks(item, parent, before))
				{
					mvApp::GetApp()->pushParent(item);
					return ToPyBool(true);
				}
			}

			else
				ThrowPythonException("add_tab was called incorrectly. Did you forget to call end_tab?");
		}

		else
		{
			auto parentItem = mvApp::GetApp()->getItem(parent);

			if (parentItem == nullptr)
			{
				ThrowPythonException("add_tab parent must exist.");
				return ToPyBool(false);
			}

			else if (parentItem->getType() == mvAppItemType::TabBar)
			{
				mvAppItem* item = new mvTab(name);
				item->checkConfigDict(kwargs);
				item->setConfigDict(kwargs);
				item->setExtraConfigDict(kwargs);
				if (AddItemWithRuntimeChecks(item, parent, before))
				{
					mvApp::GetApp()->pushParent(item);
					return ToPyBool(true);
				}
			}

			else
			{
				ThrowPythonException("add_tab parent must be a tab bar.");
				return ToPyBool(false);
			}
		}

		return ToPyBool(false);
	}

	PyObject* add_group(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int show = true;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		int width = 0;
		int horizontal = false;
		float horizontal_spacing = -1.0f;
		const char* popup = "";

		if (!(*mvApp::GetApp()->getParsers())["add_group"].parse(args, kwargs, __FUNCTION__, &name,
			&show, &tip, &parent, &before, &width, &horizontal, &horizontal_spacing, &popup))
			return ToPyBool(false);

		mvAppItem* item = new mvGroup(name);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		if (AddItemWithRuntimeChecks(item, parent, before))
		{
			mvApp::GetApp()->pushParent(item);
			if (!show)
				item->hide();

			return ToPyBool(true);
		}
		return ToPyBool(false);
	}

	PyObject* add_child(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int show = true;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		int width = 0;
		int height = 0;
		int border = true;
		const char* popup = "";

		if (!(*mvApp::GetApp()->getParsers())["add_child"].parse(args, kwargs, __FUNCTION__, &name,
			&show, &tip, &parent, &before, &width, &height, &border, &popup))
			return ToPyBool(false);

		mvAppItem* item = new mvChild(name);
		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);
		if (AddItemWithRuntimeChecks(item, parent, before))
		{
			mvApp::GetApp()->pushParent(item);
			return ToPyBool(true);
		}

		return ToPyBool(false);
	}

	PyObject* add_window(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int width = -1;
		int height = -1;
		int x_pos = 200;
		int y_pos = 200;
		int autosize = false;
		int no_resize = false;
		int no_title_bar = false;
		int no_move = false;
		const char* label = "";
		int show = true;
		PyObject* closing_callback = nullptr;

		ImGuiWindowFlags flags = ImGuiWindowFlags_NoSavedSettings;

		if (!(*mvApp::GetApp()->getParsers())["add_window"].parse(args, kwargs, __FUNCTION__, &name, &width,
			&height, &x_pos, &y_pos, &autosize, &no_resize, &no_title_bar, &no_move,
			&label, &show, &closing_callback))
			return ToPyBool(false);

		mvAppItem* item = new mvWindowAppitem(name, false, closing_callback);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		if (AddItemWithRuntimeChecks(item, "", ""))
		{
			mvApp::GetApp()->pushParent(item);

			if (!show)
				item->hide();

			return ToPyBool(true);
		}

		return ToPyBool(false);
	}

	PyObject* add_tooltip(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* tipparent;
		const char* name;
		const char* parent = "";
		const char* before = "";
		int show = true;

		if (!(*mvApp::GetApp()->getParsers())["add_tooltip"].parse(args, kwargs, __FUNCTION__, &tipparent,
			&name, &parent, &before, &show))
			return ToPyBool(false);

		mvAppItem* item = new mvTooltip(name);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		if (AddItemWithRuntimeChecks(item, tipparent, before))
		{
			mvApp::GetApp()->pushParent(item);
			return ToPyBool(true);
		}
		return ToPyBool(false);
	}

	PyObject* add_popup(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* popupparent;
		const char* name;
		int mousebutton = 1;
		int modal = false;
		const char* parent = "";
		const char* before = "";
		int width = 0;
		int height = 0;
		int show = true;

		if (!(*mvApp::GetApp()->getParsers())["add_popup"].parse(args, kwargs, __FUNCTION__, &popupparent,
			&name, &mousebutton, &modal, &parent, &before, &width, &height, &show))
			return ToPyBool(false);

		auto PopupParent = mvApp::GetApp()->getItem(popupparent);
		if (PopupParent)
			PopupParent->setPopup(name);

		if (std::string(popupparent) == "")
			mvApp::GetApp()->getItem("MainWindow")->setPopup(name);

		mvAppItem* item = new mvPopup(name);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		if (AddItemWithRuntimeChecks(item, popupparent, before))
		{
			mvApp::GetApp()->pushParent(item);
			return ToPyBool(true);
		}
		return ToPyBool(false);
	}

	PyObject* end(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvApp::GetApp()->popParent();
		return GetPyNone();
	}

	PyObject* add_collapsing_header(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int default_open = false;
		int closable = false;
		const char* label = "";
		int show = true;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";

		if (!(*mvApp::GetApp()->getParsers())["add_collapsing_header"].parse(args, kwargs, __FUNCTION__, &name,
			&default_open, &closable, &label, &show, &tip, &parent, &before))
			return ToPyBool(false);

		mvAppItem* item = new mvCollapsingHeader(name);
		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		if (AddItemWithRuntimeChecks(item, parent, before))
		{
			mvApp::GetApp()->pushParent(item);
			return ToPyBool(true);
		}
		return ToPyBool(false);
	}

	PyObject* add_tree_node(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int default_open = false;
		const char* label = "";
		int show = false;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";

		if (!(*mvApp::GetApp()->getParsers())["add_tree_node"].parse(args, kwargs, __FUNCTION__, &name,
			&default_open, &label, &show, &tip, &parent, &before))
			return ToPyBool(false);

		mvAppItem* item = new mvTreeNode(name);
		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);
		if (AddItemWithRuntimeChecks(item, parent, before))
		{
			mvApp::GetApp()->pushParent(item);
			return ToPyBool(true);
		}
		return ToPyBool(false);
	}
}
