#include "mvContainerInterface.h"
#include "mvInterfaceCore.h"

namespace Marvel {

	void AddContainerWidgets(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_menu_bar", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
		}, "Adds a menu bar to a window. Must be followed by a call to end_menu_bar.", "None", "Containers") });

		parsers->insert({ "add_menu", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
		}, "Adds a menu to an existing menu bar. Must be followed by a call to end_menu.", "None", "Containers") });

		parsers->insert({ "add_menu_item", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "shortcut", "Adds a shortcut"},
			{mvPythonDataType::Bool, "check", "Makes menu with checkmarks."},
			{mvPythonDataType::Object, "callback", "Registers a callback"},
			{mvPythonDataType::Object, "callback_data", "Callback data"},
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
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::String, "data_source", "data source for shared data"},
		}, "Adds a tab bar.", "None", "Containers") });

		parsers->insert({ "add_tab", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "closable"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
		}, "Adds a tab to a tab bar. Must be closed with the end_tab command.", "None", "Containers") });

		parsers->insert({ "add_collapsing_header", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "default_open"},
			{mvPythonDataType::Bool, "closable"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"}
		}, "Adds a collapsing header to add items to. Must be closed with the end_collapsing_header command.",
			"None", "Containers") });

		parsers->insert({ "add_tree_node", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "default_open"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
		}, "Adds a tree node to add items to. Must be closed with the end_tree_node command.",
		"None", "Containers") });

		parsers->insert({ "add_group", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Bool, "hide"},
			{mvPythonDataType::Bool, "horizontal"},
			{mvPythonDataType::Float, "horizontal_spacing",""},
		}, "Creates a group that other widgets can belong to. The group allows item commands to be issued for all of its members.\
				Must be closed with the end_group command."
		, "None", "Containers") });

		parsers->insert({ "add_child", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"},
			{mvPythonDataType::Integer, "width",""},
			{mvPythonDataType::Integer, "height", ""},
			{mvPythonDataType::Bool, "border", ""},
		}, "Adds an embedded child window. Will show scrollbars when items do not fit. Must be followed by a call to end_child.",
		"None", "Containers") });

		parsers->insert({ "add_window", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Optional},
			{mvPythonDataType::Integer, "width"},
			{mvPythonDataType::Integer, "height"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "x_pos", "x position the window will start at"},
			{mvPythonDataType::Integer, "y_pos", "y position the window will start at"},
			{mvPythonDataType::Bool, "autosize", "Autosized the window to fit it's items."},
			{mvPythonDataType::Bool, "no_resize", "Allows for the window size to be changed or fixed"},
			{mvPythonDataType::Bool, "no_title_bar", "Title name for the title bar of the window"},
			{mvPythonDataType::Bool, "no_move", "Allows for the window's position to be changed or fixed"},
			{mvPythonDataType::Bool, "show", "sets if the item is shown or not window."},
			{mvPythonDataType::Object, "on_close", "Callback ran when window is closed"},
		}, "Creates a new window for following items to be added to.",
			"None", "Containers") });

		parsers->insert({ "add_tooltip", mvPythonParser({
			{mvPythonDataType::String, "tipparent", "Sets the item's tool tip to be the same as the named item's tool tip"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)"}
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
		}, "Adds a popup window for an item. This command must come immediately after the item the popup is for. Must be followed by a call to end_popup.",
		"None", "Containers") });

		parsers->insert({ "end", mvPythonParser({
		}, "Ends a container.", "None", "Containers") });

	}

	PyObject* add_menu_bar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* before = "";
		const char* parent = "";

		if (!(*mvApp::GetApp()->getParsers())["add_menu_bar"].parse(args, kwargs, __FUNCTION__, &name,
			&parent, &before))
			return ToPyBool(false);

		auto parentItem = mvApp::GetApp()->topParent();

		if (parentItem->getType() == mvAppItemType::Window)
		{
			auto window = static_cast<mvWindowAppitem*>(parentItem);
			window->addFlag(ImGuiWindowFlags_MenuBar);
			mvAppItem* item = new mvMenuBar(name);
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
		const char* tip = "";
		const char* before = "";
		const char* parent = "";

		if (!(*mvApp::GetApp()->getParsers())["add_menu"].parse(args, kwargs, __FUNCTION__, &name,
			&tip, &parent, &before))
			return ToPyBool(false);

		//auto parentItem = mvApp::GetApp()->topParent();

		mvAppItem* item = new mvMenu(name);
		item->setTip(tip);
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
		const char* tip = "";
		const char* before = "";
		const char* parent = "";

		if (!(*mvApp::GetApp()->getParsers())["add_menu_item"].parse(args, kwargs, __FUNCTION__, &name,
			&shortcut, &check, &callback, &callback_data, &tip, &parent, &before))
			return ToPyBool(false);

		mvAppItem* item = new mvMenuItem(name, shortcut, check);
		if (callback)
			Py_XINCREF(callback);
		item->setCallback(callback);
		if (callback_data)
			Py_XINCREF(callback_data);
		item->setCallbackData(callback_data);
		item->setTip(tip);
		return ToPyBool(AddItemWithRuntimeChecks(item, parent, before));
	}

	PyObject* add_tab_bar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int reorderable = false;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* parent = "";
		const char* before = "";
		const char* data_source = "";

		ImGuiTabBarFlags flags = ImGuiTabBarFlags_None;

		if (!(*mvApp::GetApp()->getParsers())["add_tab_bar"].parse(args, kwargs, __FUNCTION__, &name, &reorderable,
			&callback, &callback_data, &parent, &before, &data_source))
			return ToPyBool(false);

		if (reorderable) flags |= ImGuiTabBarFlags_Reorderable;

		mvAppItem* item = new mvTabBar(name, reorderable);

		if (callback)
			Py_XINCREF(callback);

		item->setCallback(callback);

		if (callback_data)
			Py_XINCREF(callback_data);

		item->setCallbackData(callback_data);
		item->setDataSource(data_source);

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
		const char* tip = "";
		const char* before = "";
		const char* parent = "";

		if (!(*mvApp::GetApp()->getParsers())["add_tab"].parse(args, kwargs, __FUNCTION__, &name, &closeable,
			&tip, &parent, &before))
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
				mvAppItem* item = new mvTab(name, closeable);
				item->setTip(tip);
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
				mvAppItem* item = new mvTab(name, closeable);
				item->setTip(tip);
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
		const char* tip = "";
		int width = 0;
		const char* before = "";
		const char* parent = "";
		int hide = false;
		int horizontal = false;
		float horizontal_spacing = -1.0f;

		if (!(*mvApp::GetApp()->getParsers())["add_group"].parse(args, kwargs, __FUNCTION__, &name,
			&tip, &parent, &before, &width, &hide, &horizontal, &horizontal_spacing))
			return ToPyBool(false);

		mvAppItem* item = new mvGroup(name, horizontal, horizontal_spacing);
		item->setTip(tip);
		item->setWidth(width);
		if (AddItemWithRuntimeChecks(item, parent, before))
		{
			mvApp::GetApp()->pushParent(item);
			if (hide)
				item->hide();

			return ToPyBool(true);
		}
		return ToPyBool(false);
	}

	PyObject* add_child(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* tip = "";
		int width = 0;
		int height = 0;
		const char* before = "";
		const char* parent = "";
		int border = true;

		if (!(*mvApp::GetApp()->getParsers())["add_child"].parse(args, kwargs, __FUNCTION__, &name,
			&tip, &parent, &before, &width, &height, &border))
			return ToPyBool(false);

		mvAppItem* item = new mvChild(name, border);
		item->setTip(tip);
		item->setWidth(width);
		item->setHeight(height);
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
		int show = true;
		int no_resize = false;
		int no_title_bar = false;
		int no_move = false;
		PyObject* closing_callback = nullptr;

		ImGuiWindowFlags flags = ImGuiWindowFlags_NoSavedSettings;

		if (!(*mvApp::GetApp()->getParsers())["add_window"].parse(args, kwargs, __FUNCTION__, &name, &width,
			&height, &x_pos, &y_pos, &autosize, &no_resize, &no_title_bar, &no_move,
			&show, &closing_callback))
			return ToPyBool(false);

		if (width == -1 && height == -1)
		{
			width = 500;
			height = 500;
		}

		if (autosize)   flags |= ImGuiWindowFlags_AlwaysAutoResize;
		if (no_resize) flags |= ImGuiWindowFlags_NoResize;
		if (no_title_bar) flags |= ImGuiWindowFlags_NoTitleBar;
		if (no_move)   flags |= ImGuiWindowFlags_NoMove;

		mvAppItem* item = new mvWindowAppitem(name, width, height, x_pos, y_pos,
			false, flags, closing_callback);

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

		if (!(*mvApp::GetApp()->getParsers())["add_tooltip"].parse(args, kwargs, __FUNCTION__, &tipparent,
			&name, &parent, &before))
			return ToPyBool(false);

		mvAppItem* item = new mvTooltip(name);

		if (AddItemWithRuntimeChecks(item, parent, before))
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
		int width = 0;
		int height = 0;
		const char* before = "";
		const char* parent = "";

		if (!(*mvApp::GetApp()->getParsers())["add_popup"].parse(args, kwargs, __FUNCTION__, &popupparent,
			&name, &mousebutton, &modal, &parent, &before, &width, &height))
			return ToPyBool(false);

		auto PopupParent = mvApp::GetApp()->getItem(popupparent);
		if (PopupParent)
			PopupParent->setPopup(name);

		if (std::string(popupparent) == "")
			mvApp::GetApp()->getItem("MainWindow")->setPopup(name);

		mvAppItem* item = new mvPopup(name, mousebutton, modal);
		item->setWidth(width);
		item->setHeight(height);

		if (AddItemWithRuntimeChecks(item, parent, before))
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
		int flags = 0;
		const char* tip = "";
		const char* before = "";
		const char* parent = "";

		if (!(*mvApp::GetApp()->getParsers())["add_collapsing_header"].parse(args, kwargs, __FUNCTION__, &name,
			&default_open, &closable, &tip, &parent, &before))
			return ToPyBool(false);

		if (default_open) flags |= ImGuiTreeNodeFlags_DefaultOpen;

		mvAppItem* item = new mvCollapsingHeader(name, flags, closable);
		item->setTip(tip);

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
		int flags = 0;
		const char* tip = "";
		const char* before = "";
		const char* parent = "";

		if (!(*mvApp::GetApp()->getParsers())["add_tree_node"].parse(args, kwargs, __FUNCTION__, &name,
			&default_open, &tip, &parent, &before))
			return ToPyBool(false);

		if (default_open) flags |= ImGuiTreeNodeFlags_DefaultOpen;

		mvAppItem* item = new mvTreeNode(name, flags);
		item->setTip(tip);
		if (AddItemWithRuntimeChecks(item, parent, before))
		{
			mvApp::GetApp()->pushParent(item);
			return ToPyBool(true);
		}
		return ToPyBool(false);
	}
}
