#include "mvContainerInterface.h"
#include "mvInterfaceCore.h"

namespace Marvel {

	void AddContainerWidgets(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_managed_columns", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Integer, "columns"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "border", "show border", "True"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
		}, "Adds managed columns.", "None", "Containers") });

		parsers->insert({ "add_columns", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Integer, "columns"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "border", "show border", "True"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
		}, "Sets columns.", "None", "Containers") });

		parsers->insert({ "add_next_column", mvPythonParser({
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name", "", "'next_collumn'"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
		}, "Changes to next column.", "None", "Containers") });

		parsers->insert({ "add_menu_bar", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
		}, "Adds a menu bar to a window. Must be followed by a call to end_menu_bar.", "None", "Containers") });

		parsers->insert({ "add_menu", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "enabled", "", "True"},
		}, "Adds a menu to an existing menu bar. Must be followed by a call to end_menu.", "None", "Containers") });

		parsers->insert({ "add_menu_item", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "shortcut", "Adds a shortcut", "''"},
			{mvPythonDataType::Bool, "check", "Makes menu with checkmarks.", "False"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::Bool, "enabled", "", "True"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
		}, "Adds a menu item to an existing menu.", "None", "Containers") });

		parsers->insert({ "add_tab_bar", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "reorderable", "allows for moveable tabs", "False"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
		}, "Adds a tab bar.", "None", "Containers") });

		parsers->insert({ "add_tab", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "closable", "creates a button on the tab that can hide the tab", "False"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip", "''"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
		}, "Adds a tab to a tab bar. Must be closed with the end_tab command.", "None", "Containers") });

		parsers->insert({ "add_collapsing_header", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip", "''"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "closable", "", "False"},
			{mvPythonDataType::Bool, "default_open", "", "False"},
			{mvPythonDataType::Bool, "open_on_double_click", "Need double-click to open node", "False"},
			{mvPythonDataType::Bool, "open_on_arrow", "Only open when clicking on the arrow part.", "False"},
			{mvPythonDataType::Bool, "leaf", "No collapsing, no arrow (use as a convenience for leaf nodes).", "False"},
			{mvPythonDataType::Bool, "bullet", "Display a bullet instead of arrow", "False"},
		}, "Adds a collapsing header to add items to. Must be closed with the end_collapsing_header command.",
			"None", "Containers") });

		parsers->insert({ "add_tree_node", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip", "''"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "default_open", "", "False"},
			{mvPythonDataType::Bool, "open_on_double_click", "Need double-click to open node", "False"},
			{mvPythonDataType::Bool, "open_on_arrow", "Only open when clicking on the arrow part.", "False"},
			{mvPythonDataType::Bool, "leaf", "No collapsing, no arrow (use as a convenience for leaf nodes).", "False"},
			{mvPythonDataType::Bool, "bullet", "Display a bullet instead of arrow", "False"},
		}, "Adds a tree node to add items to. Must be closed with the end_tree_node command.",
		"None", "Containers") });

		parsers->insert({ "add_group", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip", "''"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Bool, "horizontal", "", "False"},
			{mvPythonDataType::Float, "horizontal_spacing","", "-1"},
			{mvPythonDataType::String, "popup", "", "''"},
		}, "Creates a group that other widgets can belong to. The group allows item commands to be issued for all of its members.\
				Must be closed with the end_group command."
		, "None", "Containers") });

		parsers->insert({ "add_child", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::String, "tip", "Adds a simple tooltip", "''"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Integer, "height", "", "0"},
			{mvPythonDataType::Bool, "border", "", "True"},
			{mvPythonDataType::String, "popup", "", "''"},
			{mvPythonDataType::Bool, "autosize_x", "Autosize the window to fit it's items in the x.", "False"},
			{mvPythonDataType::Bool, "autosize_y", "Autosize the window to fit it's items in the y.", "False"},
			{mvPythonDataType::Bool, "no_scrollbar" ," Disable scrollbars (window can still scroll with mouse or programmatically)", "False"},
			{mvPythonDataType::Bool, "horizontal_scrollbar" ,"Allow horizontal scrollbar to appear (off by default).", "False"},
			{mvPythonDataType::Bool, "menubar", "", "False"},
		}, "Adds an embedded child window. Will show scrollbars when items do not fit. Must be followed by a call to end_child.",
		"None", "Containers") });

		parsers->insert({ "add_window", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "width", "", "-1"},
			{mvPythonDataType::Integer, "height", "", "-1"},
			{mvPythonDataType::Integer, "x_pos", "x position the window will start at", "200"},
			{mvPythonDataType::Integer, "y_pos", "y position the window will start at", "200"},
			{mvPythonDataType::Bool, "autosize", "Autosized the window to fit it's items.", "False"},
			{mvPythonDataType::Bool, "no_resize", "Allows for the window size to be changed or fixed", "False"},
			{mvPythonDataType::Bool, "no_title_bar", "Title name for the title bar of the window", "False"},
			{mvPythonDataType::Bool, "no_move", "Allows for the window's position to be changed or fixed", "False"},
			{mvPythonDataType::Bool, "no_scrollbar" ," Disable scrollbars (window can still scroll with mouse or programmatically)", "False"},
			{mvPythonDataType::Bool, "no_collapse" ,"Disable user collapsing window by double-clicking on it", "False"},
			{mvPythonDataType::Bool, "horizontal_scrollbar" ,"Allow horizontal scrollbar to appear (off by default).", "False"},
			{mvPythonDataType::Bool, "no_focus_on_appearing" ,"Disable taking focus when transitioning from hidden to visible state", "False"},
			{mvPythonDataType::Bool, "no_bring_to_front_on_focus" ,"Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)", "False"},
			{mvPythonDataType::Bool, "menubar", "", "False"},
			{mvPythonDataType::Bool, "no_close", "", "False"},
			{mvPythonDataType::Bool, "no_background", "", "False"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::Callable, "on_close", "Callback ran when window is closed", "None"},
		}, "Creates a new window for following items to be added to.",
			"None", "Containers") });

		parsers->insert({ "add_tooltip", mvPythonParser({
			{mvPythonDataType::String, "tipparent", "Sets the item's tool tip to be the same as the named item's tool tip"},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "show","Attempt to render", "True"},
		}, "Adds an advanced tool tip for an item. This command must come immediately after the item the tip is for.",
			"None", "Containers") });

		parsers->insert({ "add_popup", mvPythonParser({
			{mvPythonDataType::String, "popupparent", "Parent that the popup will be assigned to."},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "mousebutton", "The mouse code that will trigger the popup.", "1"},
			{mvPythonDataType::Bool, "modal", "", "False"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Integer, "width", "", "0"},
			{mvPythonDataType::Integer, "height", "", "0"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Adds a popup window for an item. This command must come immediately after the item the popup is for. Must be followed by a call to end_popup.",
		"None", "Containers") });

		parsers->insert({ "end", mvPythonParser({
		}, "Ends a container.", "None", "Containers") });

		parsers->insert({ "add_about_window", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "width", "", "-1"},
			{mvPythonDataType::Integer, "height", "", "-1"},
			{mvPythonDataType::Integer, "x_pos", "x position the window will start at", "200"},
			{mvPythonDataType::Integer, "y_pos", "y position the window will start at", "200"},
			{mvPythonDataType::Bool, "autosize", "Autosized the window to fit it's items.", "False"},
			{mvPythonDataType::Bool, "no_resize", "Allows for the window size to be changed or fixed", "False"},
			{mvPythonDataType::Bool, "no_title_bar", "Title name for the title bar of the window", "False"},
			{mvPythonDataType::Bool, "no_move", "Allows for the window's position to be changed or fixed", "False"},
			{mvPythonDataType::Bool, "no_scrollbar" ," Disable scrollbars (window can still scroll with mouse or programmatically)", "False"},
			{mvPythonDataType::Bool, "no_collapse" ,"Disable user collapsing window by double-clicking on it", "False"},
			{mvPythonDataType::Bool, "horizontal_scrollbar" ,"Allow horizontal scrollbar to appear (off by default).", "False"},
			{mvPythonDataType::Bool, "no_focus_on_appearing" ,"Disable taking focus when transitioning from hidden to visible state", "False"},
			{mvPythonDataType::Bool, "no_bring_to_front_on_focus" ,"Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)", "False"},
			{mvPythonDataType::Bool, "no_close", "", "False"},
			{mvPythonDataType::Bool, "no_background", "", "False"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Creates an about window.",
			"None", "Containers") });

		parsers->insert({ "add_doc_window", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "width", "", "-1"},
			{mvPythonDataType::Integer, "height", "", "-1"},
			{mvPythonDataType::Integer, "x_pos", "x position the window will start at", "200"},
			{mvPythonDataType::Integer, "y_pos", "y position the window will start at", "200"},
			{mvPythonDataType::Bool, "autosize", "Autosized the window to fit it's items.", "True"},
			{mvPythonDataType::Bool, "no_resize", "Allows for the window size to be changed or fixed", "False"},
			{mvPythonDataType::Bool, "no_title_bar", "Title name for the title bar of the window", "False"},
			{mvPythonDataType::Bool, "no_move", "Allows for the window's position to be changed or fixed", "False"},
			{mvPythonDataType::Bool, "no_scrollbar" ," Disable scrollbars (window can still scroll with mouse or programmatically)", "False"},
			{mvPythonDataType::Bool, "no_collapse" ,"Disable user collapsing window by double-clicking on it", "False"},
			{mvPythonDataType::Bool, "horizontal_scrollbar" ,"Allow horizontal scrollbar to appear (off by default).", "False"},
			{mvPythonDataType::Bool, "no_focus_on_appearing" ,"Disable taking focus when transitioning from hidden to visible state", "False"},
			{mvPythonDataType::Bool, "no_bring_to_front_on_focus" ,"Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)", "False"},
			{mvPythonDataType::Bool, "no_close", "", "False"},
			{mvPythonDataType::Bool, "no_background", "", "False"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Creates a documentation window.",
			"None", "Containers") });

		parsers->insert({ "add_debug_window", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "width", "", "700"},
			{mvPythonDataType::Integer, "height", "", "500"},
			{mvPythonDataType::Integer, "x_pos", "x position the window will start at", "200"},
			{mvPythonDataType::Integer, "y_pos", "y position the window will start at", "200"},
			{mvPythonDataType::Bool, "autosize", "Autosized the window to fit it's items.", "False"},
			{mvPythonDataType::Bool, "no_resize", "Allows for the window size to be changed or fixed", "False"},
			{mvPythonDataType::Bool, "no_title_bar", "Title name for the title bar of the window", "False"},
			{mvPythonDataType::Bool, "no_move", "Allows for the window's position to be changed or fixed", "False"},
			{mvPythonDataType::Bool, "no_scrollbar" ," Disable scrollbars (window can still scroll with mouse or programmatically)", "False"},
			{mvPythonDataType::Bool, "no_collapse" ,"Disable user collapsing window by double-clicking on it", "False"},
			{mvPythonDataType::Bool, "horizontal_scrollbar" ,"Allow horizontal scrollbar to appear (off by default).", "False"},
			{mvPythonDataType::Bool, "no_focus_on_appearing" ,"Disable taking focus when transitioning from hidden to visible state", "False"},
			{mvPythonDataType::Bool, "no_bring_to_front_on_focus" ,"Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)", "False"},
			{mvPythonDataType::Bool, "no_close", "", "False"},
			{mvPythonDataType::Bool, "no_background", "", "False"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Creates a debug window.",
			"None", "Containers") });

		parsers->insert({ "add_style_window", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "width", "", "700"},
			{mvPythonDataType::Integer, "height", "", "500"},
			{mvPythonDataType::Integer, "x_pos", "x position the window will start at", "200"},
			{mvPythonDataType::Integer, "y_pos", "y position the window will start at", "200"},
			{mvPythonDataType::Bool, "autosize", "Autosized the window to fit it's items.", "False"},
			{mvPythonDataType::Bool, "no_resize", "Allows for the window size to be changed or fixed", "False"},
			{mvPythonDataType::Bool, "no_title_bar", "Title name for the title bar of the window", "False"},
			{mvPythonDataType::Bool, "no_move", "Allows for the window's position to be changed or fixed", "False"},
			{mvPythonDataType::Bool, "no_scrollbar" ," Disable scrollbars (window can still scroll with mouse or programmatically)", "False"},
			{mvPythonDataType::Bool, "no_collapse" ,"Disable user collapsing window by double-clicking on it", "False"},
			{mvPythonDataType::Bool, "horizontal_scrollbar" ,"Allow horizontal scrollbar to appear (off by default).", "False"},
			{mvPythonDataType::Bool, "no_focus_on_appearing" ,"Disable taking focus when transitioning from hidden to visible state", "False"},
			{mvPythonDataType::Bool, "no_bring_to_front_on_focus" ,"Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)", "False"},
			{mvPythonDataType::Bool, "no_close", "", "False"},
			{mvPythonDataType::Bool, "no_background", "", "False"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Creates a style window.",
			"None", "Containers") });

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

			item->checkConfigDict(kwargs);
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

			item->checkConfigDict(kwargs);
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
		const char* parent = "";
		const char* before = "";
		int enabled = true;

		if (!(*mvApp::GetApp()->getParsers())["add_menu"].parse(args, kwargs, __FUNCTION__, &name,
			&label, &show, &parent, &before, &enabled))
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
		int enabled = true;
		const char* parent = "";
		const char* before = "";

		if (!(*mvApp::GetApp()->getParsers())["add_menu_item"].parse(args, kwargs, __FUNCTION__, &name,
			&shortcut, &check, &callback, &callback_data, &label, &show, &enabled, &parent, &before))
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
		int autosize_x = false;
		int autosize_y = false;
		int no_scrollbar = false;
		int horizontal_scrollbar = false;
		int menubar = false;

		if (!(*mvApp::GetApp()->getParsers())["add_child"].parse(args, kwargs, __FUNCTION__, &name,
			&show, &tip, &parent, &before, &width, &height, &border, &popup, &autosize_x, 
			&autosize_y, &no_scrollbar, &horizontal_scrollbar, &menubar))
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

	PyObject* add_managed_columns(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int columns;
		int border = true;
		int show = true;
		const char* parent = "";
		const char* before = "";
		

		if (!(*mvApp::GetApp()->getParsers())["add_managed_columns"].parse(args, kwargs, __FUNCTION__, 
			&name, &columns, &border, &show, &parent, &before))
			return ToPyBool(false);

		mvAppItem* item = new mvManagedColumns(name, columns);
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

	PyObject* add_columns(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int columns;
		int border = true;
		int show = true;
		const char* parent = "";
		const char* before = "";


		if (!(*mvApp::GetApp()->getParsers())["add_columns"].parse(args, kwargs, __FUNCTION__,
			&name, &columns, &border, &show, &parent, &before))
			return ToPyBool(false);

		mvAppItem* item = new mvColumn(name, columns);
		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);
		if (AddItemWithRuntimeChecks(item, parent, before))
			return ToPyBool(true);

		return ToPyBool(false);
	}

	PyObject* add_next_column(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string("next_column" + std::to_string(i));
		const char* name = sname.c_str();
		const char* before = "";
		const char* parent = "";
		int show = true;

		if (!(*mvApp::GetApp()->getParsers())["add_next_column"].parse(args, kwargs, __FUNCTION__,
			&name, &show, &parent, &before))
			return ToPyBool(false);

		mvAppItem* item = new mvNextColumn(name);
		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);
		if (AddItemWithRuntimeChecks(item, parent, before))
			return ToPyBool(true);

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
		int no_scrollbar = false;
		int no_collapse = false;
		int horizontal_scrollbar = false;
		int no_focus_on_appearing = false;
		int no_bring_to_front_on_focus = false;
		int menubar = false;
		int noclose = false;
		int no_background = false;

		const char* label = "";
		int show = true;
		PyObject* closing_callback = nullptr;

		//ImGuiWindowFlags flags = ImGuiWindowFlags_NoSavedSettings;

		if (!(*mvApp::GetApp()->getParsers())["add_window"].parse(args, kwargs, __FUNCTION__, &name, &width,
			&height, &x_pos, &y_pos, &autosize, &no_resize, &no_title_bar, &no_move, &no_scrollbar, 
			&no_collapse, &horizontal_scrollbar, &no_focus_on_appearing, &no_bring_to_front_on_focus, &menubar,
			&noclose, &no_background, &label, &show, &closing_callback))
			return ToPyBool(false);

		if (closing_callback)
			Py_XINCREF(closing_callback);

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

	PyObject* add_about_window(PyObject* self, PyObject* args, PyObject* kwargs)
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
		int no_scrollbar = false;
		int no_collapse = false;
		int horizontal_scrollbar = false;
		int no_focus_on_appearing = false;
		int no_bring_to_front_on_focus = false;
		int noclose = false;
		int no_background = false;

		const char* label = "";
		int show = true;

		if (!(*mvApp::GetApp()->getParsers())["add_about_window"].parse(args, kwargs, __FUNCTION__, &name, &width,
			&height, &x_pos, &y_pos, &autosize, &no_resize, &no_title_bar, &no_move, &no_scrollbar,
			&no_collapse, &horizontal_scrollbar, &no_focus_on_appearing, &no_bring_to_front_on_focus,
			&noclose, &no_background, &label, &show))
			return ToPyBool(false);

		mvAppItem* item = new mvAboutWindow(name);

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

	PyObject* add_doc_window(PyObject* self, PyObject* args, PyObject* kwargs)
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
		int no_scrollbar = false;
		int no_collapse = false;
		int horizontal_scrollbar = false;
		int no_focus_on_appearing = false;
		int no_bring_to_front_on_focus = false;
		int noclose = false;
		int no_background = false;

		const char* label = "";
		int show = true;

		if (!(*mvApp::GetApp()->getParsers())["add_doc_window"].parse(args, kwargs, __FUNCTION__, &name, &width,
			&height, &x_pos, &y_pos, &autosize, &no_resize, &no_title_bar, &no_move, &no_scrollbar,
			&no_collapse, &horizontal_scrollbar, &no_focus_on_appearing, &no_bring_to_front_on_focus,
			&noclose, &no_background, &label, &show))
			return ToPyBool(false);

		mvAppItem* item = new mvDocWindow(name);

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

	PyObject* add_debug_window(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int width = 700;
		int height = 500;
		int x_pos = 200;
		int y_pos = 200;
		int autosize = false;
		int no_resize = false;
		int no_title_bar = false;
		int no_move = false;
		int no_scrollbar = false;
		int no_collapse = false;
		int horizontal_scrollbar = false;
		int no_focus_on_appearing = false;
		int no_bring_to_front_on_focus = false;
		int noclose = false;
		int no_background = false;

		const char* label = "";
		int show = true;

		if (!(*mvApp::GetApp()->getParsers())["add_debug_window"].parse(args, kwargs, __FUNCTION__, &name, &width,
			&height, &x_pos, &y_pos, &autosize, &no_resize, &no_title_bar, &no_move, &no_scrollbar,
			&no_collapse, &horizontal_scrollbar, &no_focus_on_appearing, &no_bring_to_front_on_focus,
			&noclose, &no_background, &label, &show))
			return ToPyBool(false);

		mvAppItem* item = new mvDebugWindow(name);

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

	PyObject* add_style_window(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int width = 700;
		int height = 500;
		int x_pos = 200;
		int y_pos = 200;
		int autosize = false;
		int no_resize = false;
		int no_title_bar = false;
		int no_move = false;
		int no_scrollbar = false;
		int no_collapse = false;
		int horizontal_scrollbar = false;
		int no_focus_on_appearing = false;
		int no_bring_to_front_on_focus = false;
		int noclose = false;
		int no_background = false;

		const char* label = "";
		int show = true;

		if (!(*mvApp::GetApp()->getParsers())["add_style_window"].parse(args, kwargs, __FUNCTION__, &name, &width,
			&height, &x_pos, &y_pos, &autosize, &no_resize, &no_title_bar, &no_move, &no_scrollbar,
			&no_collapse, &horizontal_scrollbar, &no_focus_on_appearing, &no_bring_to_front_on_focus,
			&noclose, &no_background, &label, &show))
			return ToPyBool(false);

		mvAppItem* item = new mvStyleWindow(name);

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

		mvAppItem* item = new mvPopup(name, PopupParent);

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
		const char* label = "";
		int show = true;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		int closable = false;
		int default_open = false;
		int open_on_double_click = false;
		int open_on_arrow = false;
		int leaf = false;
		int bullet = false;


		if (!(*mvApp::GetApp()->getParsers())["add_collapsing_header"].parse(args, kwargs, __FUNCTION__, &name,
			&label, &show, &tip, &parent, &before, &closable, &default_open, &open_on_double_click, &open_on_arrow, &leaf, &bullet))
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
		const char* label = "";
		int show = false;
		const char* tip = "";
		const char* parent = "";
		const char* before = "";
		int default_open = false;
		int open_on_double_click = false;
		int open_on_arrow = false;
		int leaf = false;
		int bullet = false;

		if (!(*mvApp::GetApp()->getParsers())["add_tree_node"].parse(args, kwargs, __FUNCTION__, &name,
			&label, &show, &tip, &parent, &before, &default_open, &open_on_double_click, &open_on_arrow, &leaf, &bullet))
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
