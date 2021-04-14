#include "mvWindowAppItem.h"
#include "mvInput.h"
#include "mvViewport.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvWindowAppItem::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("parent");
		parser.removeArg("before");
		parser.removeArg("source");
		parser.removeArg("callback");
		parser.removeArg("callback_data");
		parser.removeArg("enabled");

		parser.addArg<mvPyDataType::Integer>("x_pos", mvArgType::KEYWORD_ARG, "200");
		parser.addArg<mvPyDataType::Integer>("y_pos", mvArgType::KEYWORD_ARG, "200");

		parser.addArg<mvPyDataType::IntList>("min_size", mvArgType::KEYWORD_ARG, "[32, 32]", "Minimum window size.");
		parser.addArg<mvPyDataType::IntList>("max_size", mvArgType::KEYWORD_ARG, "[30000, 30000]", "Maximum window size.");

		parser.addArg<mvPyDataType::Bool>("menubar", mvArgType::KEYWORD_ARG, "False");
		parser.addArg<mvPyDataType::Bool>("collapsed", mvArgType::KEYWORD_ARG, "False", "Collapse the window");
		parser.addArg<mvPyDataType::Bool>("autosize", mvArgType::KEYWORD_ARG, "False", "Autosized the window to fit it's items.");
		parser.addArg<mvPyDataType::Bool>("no_resize", mvArgType::KEYWORD_ARG, "False", "Allows for the window size to be changed or fixed");
		parser.addArg<mvPyDataType::Bool>("no_title_bar", mvArgType::KEYWORD_ARG, "False", "Title name for the title bar of the window");
		parser.addArg<mvPyDataType::Bool>("no_move", mvArgType::KEYWORD_ARG, "False", "Allows for the window's position to be changed or fixed");
		parser.addArg<mvPyDataType::Bool>("no_scrollbar", mvArgType::KEYWORD_ARG, "False", " Disable scrollbars (window can still scroll with mouse or programmatically)");
		parser.addArg<mvPyDataType::Bool>("no_collapse", mvArgType::KEYWORD_ARG, "False", "Disable user collapsing window by double-clicking on it");
		parser.addArg<mvPyDataType::Bool>("horizontal_scrollbar", mvArgType::KEYWORD_ARG, "False", "Allow horizontal scrollbar to appear (off by default).");
		parser.addArg<mvPyDataType::Bool>("no_focus_on_appearing", mvArgType::KEYWORD_ARG, "False", "Disable taking focus when transitioning from hidden to visible state");
		parser.addArg<mvPyDataType::Bool>("no_bring_to_front_on_focus", mvArgType::KEYWORD_ARG, "False", "Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)");
		parser.addArg<mvPyDataType::Bool>("no_close", mvArgType::KEYWORD_ARG, "False");
		parser.addArg<mvPyDataType::Bool>("no_background", mvArgType::KEYWORD_ARG, "False");

		parser.addArg<mvPyDataType::Callable>("on_close", mvArgType::KEYWORD_ARG, "None", "Callback ran when window is closed");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvWindowAppItem::mvWindowAppItem(const std::string& name, bool mainWindow)
		: mvAppItem(name), m_mainWindow(mainWindow)
	{

		m_width = 500;
		m_height = 500;

		m_oldWindowflags = ImGuiWindowFlags_NoSavedSettings;

		if (mainWindow)
		{
			m_windowflags = ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoSavedSettings
				| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;
		}
	}

	mvWindowAppItem::~mvWindowAppItem()
	{
		PyObject* callback = m_on_close;
		mvApp::GetApp()->getCallbackRegistry().submitCallback([callback]() {
			if (callback)
				Py_XDECREF(callback);
			});

	}

	void mvWindowAppItem::onChildAdd(mvRef<mvAppItem> item)
	{
		if(item->getType() == mvAppItemType::mvMenuBar)
			m_windowflags |= ImGuiWindowFlags_MenuBar;
	}

	void mvWindowAppItem::onChildRemoved(mvRef<mvAppItem> item)
	{
		if (item->getType() == mvAppItemType::mvMenuBar)
			m_windowflags &= ~ImGuiWindowFlags_MenuBar;
	}

	void mvWindowAppItem::setWindowAsMainStatus(bool value)
	{
		m_mainWindow = value;
		if (value)
		{
			m_oldWindowflags = m_windowflags;
			m_windowflags = ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoSavedSettings
				| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;

			if (m_hasMenuBar)
				m_windowflags |= ImGuiWindowFlags_MenuBar;
			m_oldxpos = m_xpos;
			m_oldypos = m_ypos;
			m_oldWidth = m_width;
			m_oldHeight = m_height;
		}
		else
		{
			m_focusNextFrame = true;
			m_windowflags = m_oldWindowflags;
			if (m_hasMenuBar)
				m_windowflags |= ImGuiWindowFlags_MenuBar;
			m_xpos = m_oldxpos;
			m_ypos = m_oldypos;
			m_width = m_oldWidth;
			m_height = m_oldHeight;
			m_dirty_pos = true;
			m_dirty_size = true;
		}


	}

	void mvWindowAppItem::setWindowPos(float x, float y)
	{
		m_xpos = (int)x;
		m_ypos = (int)y;
		m_dirty_pos = true;
	}

	void mvWindowAppItem::setWidth(int width) 
	{ 
		m_width = width;
		m_dirty_size = true; 
	}

	void mvWindowAppItem::setHeight(int height) 
	{ 
		m_height = height;
		m_dirty_size = true; 
	}

	void mvWindowAppItem::setLabel(const std::string& value)
	{
		m_specificedlabel = value;
		m_label = value + "###" + m_name;
		m_dirty_pos = true;
		m_dirty_size = true;
	}

	mvVec2 mvWindowAppItem::getWindowPos() const
	{
		return { (float)m_xpos, (float)m_ypos };
	}

	void mvWindowAppItem::setResizeCallback(PyObject* callback)
	{
		m_resize_callback = callback;
	}

	void mvWindowAppItem::draw(ImDrawList* drawlist, float x, float y)
	{
		// shouldn't have to do this but do. Fix later
		if (!m_show)
		{
			m_state.setHovered(false);
			m_state.setFocused(false);
			m_state.setActivated(false);
			m_state.setVisible(false);
			if (!m_closing)
			{
				m_closing = true;
				mvApp::GetApp()->getCallbackRegistry().addCallback(m_on_close, m_name, nullptr);

			}
			return;
		}
		m_closing = false;

		if (m_mainWindow)
		{
			ImGui::SetNextWindowBgAlpha(1.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f); // to prevent main window corners from showing
			ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
			ImGui::SetNextWindowSize(ImVec2((float)mvApp::GetApp()->getViewport()->getClientWidth(), (float)mvApp::GetApp()->getViewport()->getClientHeight()));
		}

		else if (m_dirty_pos)
		{
			ImGui::SetNextWindowPos(ImVec2((float)m_xpos, (float)m_ypos));
			m_dirty_pos = false;
		}

		else if (m_focusNextFrame)
		{
			ImGui::SetNextWindowFocus();
			m_focusNextFrame = false;
		}

		if (m_dirty_size)
		{
			ImGui::SetNextWindowSize(ImVec2((float)m_width, (float)m_height));
			m_dirty_size = false;
		}

		if (m_collapsedDirty)
		{
			ImGui::SetNextWindowCollapsed(m_collapsed);
			m_collapsedDirty = false;
		}

		ImGui::SetNextWindowSizeConstraints(m_min_size, m_max_size);

		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (!ImGui::Begin(m_label.c_str(), m_no_close ? nullptr : &m_show, m_windowflags))
		{
			if (m_mainWindow)
				ImGui::PopStyleVar();

			ImGui::End();
			return;
		}

		ImDrawList* this_drawlist = ImGui::GetWindowDrawList();

		float startx = (float)ImGui::GetCursorScreenPos().x;
		float starty = (float)ImGui::GetCursorScreenPos().y;

		//we do this so that the children dont get the theme
		scope.cleanup();

		if (m_mainWindow)
			ImGui::PopStyleVar();

		for (auto& item : m_children[0])
		{
			// skip item if it's not shown
			if (!item->m_show)
				continue;

			item->draw(this_drawlist, startx, starty);

			item->getState().update();

		}

		for (auto& item : m_children[1])
		{
			// skip item if it's not shown
			if (!item->m_show)
				continue;

			// set item width
			if (item->m_width != 0)
				ImGui::SetNextItemWidth((float)item->m_width);

			item->draw(this_drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

			item->getState().update();

		}




		m_state.setVisible(true);
		m_state.setHovered(ImGui::IsWindowHovered());
		m_state.setFocused(ImGui::IsWindowFocused());
		m_state.setRectSize({ ImGui::GetWindowSize().x, ImGui::GetWindowSize().y });
		m_state.setActivated(ImGui::IsWindowCollapsed());

		if (ImGui::GetWindowWidth() != (float)m_width || ImGui::GetWindowHeight() != (float)m_height)
		{
			m_width = (int)ImGui::GetWindowWidth();
			m_height = (int)ImGui::GetWindowHeight();
			mvApp::GetApp()->getCallbackRegistry().addCallback(m_resize_callback, m_name, nullptr);
		}

		m_width = (int)ImGui::GetWindowWidth();
		m_height = (int)ImGui::GetWindowHeight();

		if (m_state.isItemFocused())
		{

			float titleBarHeight = ImGui::GetStyle().FramePadding.y * 2 + ImGui::GetFontSize();

			// update mouse
			ImVec2 mousePos = ImGui::GetMousePos();
			float x = mousePos.x - ImGui::GetWindowPos().x;
			float y = mousePos.y - ImGui::GetWindowPos().y - titleBarHeight;
			mvInput::setMousePosition(x, y);

			if (mvApp::GetApp()->getItemRegistry().getActiveWindow() != m_name)
				mvEventBus::Publish(mvEVT_CATEGORY_ITEM, mvEVT_ACTIVE_WINDOW, { CreateEventArgument("WINDOW", m_name) });

		}

		m_xpos = (int)ImGui::GetWindowPos().x;
		m_ypos = (int)ImGui::GetWindowPos().y;

		ImGui::End();

		m_collapsed = ImGui::IsWindowCollapsed();

	}

	void mvWindowAppItem::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "x_pos")) setWindowPos((float)ToInt(item), (float)m_ypos);
		if (PyObject* item = PyDict_GetItemString(dict, "y_pos")) setWindowPos((float)m_xpos, (float)ToInt(item));
		if (PyObject* item = PyDict_GetItemString(dict, "no_close")) m_no_close = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "collapsed"))
		{
			m_collapsedDirty = true;
			m_collapsed = ToBool(item);
		}

		if (PyObject* item = PyDict_GetItemString(dict, "min_size"))
		{
			auto min_size = ToIntVect(item);
			m_min_size = { (float)min_size[0], (float)min_size[1]};
		}

		if (PyObject* item = PyDict_GetItemString(dict, "max_size"))
		{
			auto max_size = ToIntVect(item);
			m_max_size = { (float)max_size[0], (float)max_size[1] };
		}

		if (PyObject* item = PyDict_GetItemString(dict, "on_close"))
		{
			if (m_on_close)
				Py_XDECREF(m_on_close);
			item = SanitizeCallback(item);
			if (item)
				Py_XINCREF(item);
			m_on_close = item;
		}

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// window flags
		flagop("autosize", ImGuiWindowFlags_AlwaysAutoResize, m_windowflags);
		flagop("no_move", ImGuiWindowFlags_NoMove, m_windowflags);
		flagop("no_resize", ImGuiWindowFlags_NoResize, m_windowflags);
		flagop("no_title_bar", ImGuiWindowFlags_NoTitleBar, m_windowflags);
		flagop("no_scrollbar", ImGuiWindowFlags_NoScrollbar, m_windowflags);
		flagop("no_collapse", ImGuiWindowFlags_NoCollapse, m_windowflags);
		flagop("horizontal_scrollbar", ImGuiWindowFlags_HorizontalScrollbar, m_windowflags);
		flagop("no_focus_on_appearing", ImGuiWindowFlags_NoFocusOnAppearing, m_windowflags);
		flagop("no_bring_to_front_on_focus", ImGuiWindowFlags_NoBringToFrontOnFocus, m_windowflags);
		flagop("menubar", ImGuiWindowFlags_MenuBar, m_windowflags);
		flagop("no_background", ImGuiWindowFlags_NoBackground, m_windowflags);

		m_oldxpos = m_xpos;
		m_oldypos = m_ypos;
		m_oldWidth = m_width;
		m_oldHeight = m_height;
		m_oldWindowflags = m_windowflags;

	}

	void mvWindowAppItem::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "x_pos", ToPyInt(m_xpos));
		PyDict_SetItemString(dict, "y_pos", ToPyInt(m_ypos));
		PyDict_SetItemString(dict, "no_close", ToPyBool(m_no_close));
		PyDict_SetItemString(dict, "collapsed", ToPyBool(m_collapsed));
		PyDict_SetItemString(dict, "min_size", ToPyPair(m_min_size.x, m_min_size.y));
		PyDict_SetItemString(dict, "max_size", ToPyPair(m_min_size.x, m_min_size.y));

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		// window flags
		checkbitset("autosize", ImGuiWindowFlags_AlwaysAutoResize, m_windowflags);
		checkbitset("no_resize", ImGuiWindowFlags_NoResize, m_windowflags);
		checkbitset("no_title_bar", ImGuiWindowFlags_NoTitleBar, m_windowflags);
		checkbitset("no_move", ImGuiWindowFlags_NoMove, m_windowflags);
		checkbitset("no_scrollbar", ImGuiWindowFlags_NoScrollbar, m_windowflags);
		checkbitset("no_collapse", ImGuiWindowFlags_NoCollapse, m_windowflags);
		checkbitset("horizontal_scrollbar", ImGuiWindowFlags_HorizontalScrollbar, m_windowflags);
		checkbitset("no_focus_on_appearing", ImGuiWindowFlags_NoFocusOnAppearing, m_windowflags);
		checkbitset("no_bring_to_front_on_focus", ImGuiWindowFlags_NoBringToFrontOnFocus, m_windowflags);
		checkbitset("menubar", ImGuiWindowFlags_MenuBar, m_windowflags);
		checkbitset("no_background", ImGuiWindowFlags_NoBackground, m_windowflags);
	}

}