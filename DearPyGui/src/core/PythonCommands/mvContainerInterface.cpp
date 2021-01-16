#include "mvContainerInterface.h"

namespace Marvel {

	void AddContainerWidgets(std::map<std::string, mvPythonParser>* parsers)
	{

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
		}, "Adds a popup window for an item. This command must come immediately after the item the popup is for. Must be followed by a call to end.",
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

		parsers->insert({ "add_metrics_window", mvPythonParser({
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
		}, "Creates a metrics window.",
	"None", "Containers") });

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

		auto item = CreateRef<mvAboutWindow>(name);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		if (mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, "", ""))
		{
			mvApp::GetApp()->getItemRegistry().pushParent(item);
			if (!show)
				item->hide();

		}

		return GetPyNone();
	}

	PyObject* add_metrics_window(PyObject* self, PyObject* args, PyObject* kwargs)
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

		if (!(*mvApp::GetApp()->getParsers())["add_metrics_window"].parse(args, kwargs, __FUNCTION__, &name, &width,
			&height, &x_pos, &y_pos, &autosize, &no_resize, &no_title_bar, &no_move, &no_scrollbar,
			&no_collapse, &horizontal_scrollbar, &no_focus_on_appearing, &no_bring_to_front_on_focus,
			&noclose, &no_background, &label, &show))
			return ToPyBool(false);

		auto item = CreateRef<mvMetricsWindow>(name);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		if (mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, "", ""))
		{
			mvApp::GetApp()->getItemRegistry().pushParent(item);
			if (!show)
				item->hide();

		}

		return GetPyNone();
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

		auto PopupParent = mvApp::GetApp()->getItemRegistry().getItem(popupparent);

		auto item = CreateRef<mvPopup>(name, PopupParent.get());

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		if (mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, popupparent, before))
		{
			mvApp::GetApp()->getItemRegistry().pushParent(item);
			if (!show)
				item->hide();

		}

		return GetPyNone();
	}

	PyObject* end(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		mvApp::GetApp()->getItemRegistry().popParent();
		return GetPyNone();
	}
}
