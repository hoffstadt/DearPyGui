#include "mvWindowAppItem.h"
#include "mvInput.h"
#include "mvViewport.h"
#include "mvItemRegistry.h"
#include "mvLog.h"
#include "mvPythonExceptions.h"
#include "mvChild.h"
#include "fonts/mvFont.h"
#include "themes/mvTheme.h"

namespace Marvel {

	void mvWindowAppItem::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		{
			mvPythonParser parser(mvPyDataType::UUID, "Creates a new window for following items to be added to.", { "Containers", "Widgets" }, true);
			mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
				MV_PARSER_ARG_ID |
				MV_PARSER_ARG_WIDTH |
				MV_PARSER_ARG_HEIGHT |
				MV_PARSER_ARG_INDENT |
				MV_PARSER_ARG_SEARCH_DELAY |
				MV_PARSER_ARG_SHOW |
				MV_PARSER_ARG_POS)
			);

			parser.addArg<mvPyDataType::IntList>("min_size", mvArgType::KEYWORD_ARG, "[100, 100]", "Minimum window size.");
			parser.addArg<mvPyDataType::IntList>("max_size", mvArgType::KEYWORD_ARG, "[30000, 30000]", "Maximum window size.");

			parser.addArg<mvPyDataType::Bool>("menubar", mvArgType::KEYWORD_ARG, "False", "Shows or hides the menubar.");
			parser.addArg<mvPyDataType::Bool>("collapsed", mvArgType::KEYWORD_ARG, "False", "Collapse the window.");
			parser.addArg<mvPyDataType::Bool>("autosize", mvArgType::KEYWORD_ARG, "False", "Autosized the window to fit it's items.");
			parser.addArg<mvPyDataType::Bool>("no_resize", mvArgType::KEYWORD_ARG, "False", "Allows for the window size to be changed or fixed.");
			parser.addArg<mvPyDataType::Bool>("no_title_bar", mvArgType::KEYWORD_ARG, "False", "Title name for the title bar of the window.");
			parser.addArg<mvPyDataType::Bool>("no_move", mvArgType::KEYWORD_ARG, "False", "Allows for the window's position to be changed or fixed.");
			parser.addArg<mvPyDataType::Bool>("no_scrollbar", mvArgType::KEYWORD_ARG, "False", " Disable scrollbars. (window can still scroll with mouse or programmatically)");
			parser.addArg<mvPyDataType::Bool>("no_collapse", mvArgType::KEYWORD_ARG, "False", "Disable user collapsing window by double-clicking on it.");
			parser.addArg<mvPyDataType::Bool>("horizontal_scrollbar", mvArgType::KEYWORD_ARG, "False", "Allow horizontal scrollbar to appear. (off by default)");
			parser.addArg<mvPyDataType::Bool>("no_focus_on_appearing", mvArgType::KEYWORD_ARG, "False", "Disable taking focus when transitioning from hidden to visible state.");
			parser.addArg<mvPyDataType::Bool>("no_bring_to_front_on_focus", mvArgType::KEYWORD_ARG, "False", "Disable bringing window to front when taking focus. (e.g. clicking on it or programmatically giving it focus)");
			parser.addArg<mvPyDataType::Bool>("no_close", mvArgType::KEYWORD_ARG, "False", "Disable user closing the window by removing the close button.");
			parser.addArg<mvPyDataType::Bool>("no_background", mvArgType::KEYWORD_ARG, "False", "Sets Background and border alpha to transparent.");
			parser.addArg<mvPyDataType::Bool>("modal", mvArgType::KEYWORD_ARG, "False", "Fills area behind window according to the theme and disables user ability to interact with anything except the window.");
			parser.addArg<mvPyDataType::Bool>("popup", mvArgType::KEYWORD_ARG, "False", "Fills area behind window according to the theme, removes title bar, collapse and close. Window can be closed by selecting area in the background behind the window.");
			parser.addArg<mvPyDataType::Bool>("no_saved_settings", mvArgType::KEYWORD_ARG, "False", "Never load/save settings in .ini file.");
			
			parser.addArg<mvPyDataType::Callable>("on_close", mvArgType::KEYWORD_ARG, "None", "Callback ran when window is closed.");

			parser.finalize();

			parsers->insert({ s_command, parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.addArg<mvPyDataType::UUID>("item");
			parser.addArg<mvPyDataType::Float>("value");
			parser.finalize();
			parsers->insert({ "set_x_scroll", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.addArg<mvPyDataType::UUID>("item");
			parser.addArg<mvPyDataType::Float>("value");
			parser.finalize();
			parsers->insert({ "set_y_scroll", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::Float);
			parser.addArg<mvPyDataType::UUID>("item");
			parser.finalize();
			parsers->insert({ "get_x_scroll", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::Float);
			parser.addArg<mvPyDataType::UUID>("item");
			parser.finalize();
			parsers->insert({ "get_y_scroll", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::Float);
			parser.addArg<mvPyDataType::UUID>("item");
			parser.finalize();
			parsers->insert({ "get_x_scroll_max", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::Float);
			parser.addArg<mvPyDataType::UUID>("item");
			parser.finalize();
			parsers->insert({ "get_y_scroll_max", parser });
		}
	}

	mvWindowAppItem::mvWindowAppItem(mvUUID uuid, bool mainWindow)
		: mvAppItem(uuid), _mainWindow(mainWindow)
	{
		//_label = "Window###" + std::to_string(_uuid);
		_width = 500;
		_height = 500;
		_dirty_size = true;

		_oldWindowflags = ImGuiWindowFlags_None;

		if (mainWindow)
		{
			_windowflags = ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoSavedSettings
				| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;
		}

	}

	mvWindowAppItem::~mvWindowAppItem()
	{
		PyObject* callback = _on_close;
		mvApp::GetApp()->getCallbackRegistry().submitCallback([callback]() {
			if (callback)
				Py_XDECREF(callback);
			});
	}

	void mvWindowAppItem::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvWindowAppItem*>(item);
		_windowflags = titem->_windowflags;
		_oldWindowflags = titem->_oldWindowflags;
		_modal = titem->_modal;
		_popup = titem->_popup;
		_autosize = titem->_autosize;
		_no_resize = titem->_no_resize;
		_no_title_bar = titem->_no_title_bar;
		_no_move = titem->_no_move;
		_no_scrollbar = titem->_no_scrollbar;
		_no_collapse = titem->_no_collapse;
		_horizontal_scrollbar = titem->_horizontal_scrollbar;
		_no_focus_on_appearing = titem->_no_focus_on_appearing;
		_no_bring_to_front_on_focus = titem->_no_bring_to_front_on_focus;
		_menubar = titem->_menubar;
		_no_close = titem->_no_close;
		_no_background = titem->_no_background;
		_collapsed = titem->_collapsed;
		_min_size = titem->_min_size;
		_max_size = titem->_max_size;

		if (titem->_on_close)
		{
			Py_XINCREF(titem->_on_close);
			_on_close = titem->_on_close;
		}
	}

	void mvWindowAppItem::onChildAdd(mvRef<mvAppItem> item)
	{
		if (item->getType() == mvAppItemType::mvMenuBar)
			_windowflags |= ImGuiWindowFlags_MenuBar;
	}

	void mvWindowAppItem::onChildRemoved(mvRef<mvAppItem> item)
	{
		if (item->getType() == mvAppItemType::mvMenuBar)
			_windowflags &= ~ImGuiWindowFlags_MenuBar;
	}

	void mvWindowAppItem::setWindowAsMainStatus(bool value)
	{
		_mainWindow = value;
		if (value)
		{
			_oldWindowflags = _windowflags;
			_windowflags = ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoSavedSettings
				| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;

			if (_oldWindowflags & ImGuiWindowFlags_MenuBar)
				_windowflags |= ImGuiWindowFlags_MenuBar;
			_oldxpos = _state.getItemPos().x;
			_oldypos = _state.getItemPos().y;
			_oldWidth = _width;
			_oldHeight = _height;
		}
		else
		{
			_focusNextFrame = true;
			if (_windowflags & ImGuiWindowFlags_MenuBar)
				_oldWindowflags |= ImGuiWindowFlags_MenuBar;
			_windowflags = _oldWindowflags;
			if (_windowflags & ImGuiWindowFlags_MenuBar)
				_windowflags |= ImGuiWindowFlags_MenuBar;
			_state.setPos({ _oldxpos , _oldypos });
			_width = _oldWidth;
			_height = _oldHeight;
			_dirtyPos = true;
			_dirty_size = true;
		}


	}

	void mvWindowAppItem::draw(ImDrawList* drawlist, float x, float y)
	{

		//-----------------------------------------------------------------------------
		// pre draw
		//-----------------------------------------------------------------------------

		if (!_show)
			return;

		if (mvApp::s_frame == 1 && mvApp::GetApp()->isUsingIniFile() && !(_windowflags & ImGuiWindowFlags_NoSavedSettings))
		{
			_dirtyPos = false;
			_dirty_size = false;
			_collapsedDirty = false;
		}

		if (_focusNextFrame)
		{
			ImGui::SetNextWindowFocus();
			_focusNextFrame = false;
		}

		// handle fonts
		if (_font)
		{
			ImFont* fontptr = static_cast<mvFont*>(_font.get())->getFontPtr();
			ImGui::PushFont(fontptr);
		}

		// handle class theming
		if (auto classTheme = getClassTheme())
			static_cast<mvTheme*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);

		// handle item theming
		if (_theme)
			static_cast<mvTheme*>(_theme.get())->draw(nullptr, 0.0f, 0.0f);


		//-----------------------------------------------------------------------------
		// draw
		//-----------------------------------------------------------------------------

		ScopedID id(_uuid);

		if (_mainWindow)
		{
			ImGui::SetNextWindowBgAlpha(1.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f); // to prevent main window corners from showing
			ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
			ImGui::SetNextWindowSize(ImVec2((float)mvApp::GetApp()->getViewport()->getClientWidth(), (float)mvApp::GetApp()->getViewport()->getClientHeight()));
		}

		else if (_dirtyPos)
		{
			ImGui::SetNextWindowPos(_state.getItemPos());
			_dirtyPos = false;
		}

		if (_dirty_size)
		{
			ImGui::SetNextWindowSize(ImVec2((float)_width, (float)_height));
			_dirty_size = false;
		}

		if (_collapsedDirty)
		{
			ImGui::SetNextWindowCollapsed(_collapsed);
			_collapsedDirty = false;
		}

		ImGui::SetNextWindowSizeConstraints(_min_size, _max_size);

		if (_modal)
		{
			if (wasShownLastFrameReset())
				ImGui::OpenPopup(_internalLabel.c_str());
			
			if (!ImGui::BeginPopupModal(_internalLabel.c_str(), _no_close ? nullptr : &_show, _windowflags))
			{
				if (_mainWindow)
					ImGui::PopStyleVar();

				// shouldn't have to do this but do. Fix later
				_show = false;
				_state._lastFrameUpdate = mvApp::s_frame;
				_state._hovered = false;
				_state._focused = false;
				_state._activated = false;
				_state._visible = false;

				if(_alias.empty())
					mvApp::GetApp()->getCallbackRegistry().addCallback(_on_close, _uuid, nullptr, _user_data);
				else
					mvApp::GetApp()->getCallbackRegistry().addCallback(_on_close, _alias, nullptr, _user_data);
				
				return;
			}
		}

		else if (_popup)
		{
			if (wasShownLastFrameReset())
				ImGui::OpenPopup(_internalLabel.c_str());

			if (!ImGui::BeginPopup(_internalLabel.c_str(), _windowflags))
			{
				if (_mainWindow)
					ImGui::PopStyleVar();
				return;
			}
		}

		else
		{
			if (!ImGui::Begin(_internalLabel.c_str(), _no_close ? nullptr : &_show, _windowflags))
			{
				if (_mainWindow)
					ImGui::PopStyleVar();

				ImGui::End();
				return;
			}
		}

		ImDrawList* this_drawlist = ImGui::GetWindowDrawList();

		float startx = (float)ImGui::GetCursorScreenPos().x;
		float starty = (float)ImGui::GetCursorScreenPos().y;

		if (_mainWindow)
			ImGui::PopStyleVar();

		for (auto& item : _children[0])
		{
			// skip item if it's not shown
			if (!item->isShown())
				continue;

			item->draw(this_drawlist, startx, starty);

			item->getState().update();

		}

		for (auto& item : _children[1])
		{
			if (!item->preDraw())
				continue;

			item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
			if(item->isTracked())
				ImGui::SetScrollHereY(item->getTrackOffset());

			item->postDraw();
		}

		for (auto& item : _children[2])
		{
			// skip item if it's not shown
			if (!item->isShown())
				continue;

			item->draw(this_drawlist, startx, starty);

			item->getState().update();

		}

		//-----------------------------------------------------------------------------
		// post draw
		//-----------------------------------------------------------------------------

		// pop font from stack
		if (_font)
			ImGui::PopFont();

		// pop class theme
		if (auto classTheme = getClassTheme())
			static_cast<mvTheme*>(classTheme.get())->customAction();

		// pop item theme
		if (_theme)
			static_cast<mvTheme*>(_theme.get())->customAction();

		if (_scrollXSet)
		{
			if (_scrollX < 0.0f)
				ImGui::SetScrollHereX(1.0f);
			else
				ImGui::SetScrollX(_scrollX);
			_scrollXSet = false;
		}

		if (_scrollYSet)
		{
			if (_scrollY < 0.0f)
				ImGui::SetScrollHereY(1.0f);
			else
				ImGui::SetScrollY(_scrollY);
			_scrollYSet = false;
		}

		_scrollX = ImGui::GetScrollX();
		_scrollY = ImGui::GetScrollY();
		_scrollMaxX = ImGui::GetScrollMaxX();
		_scrollMaxY = ImGui::GetScrollMaxY();

		_state._lastFrameUpdate = mvApp::s_frame;
		_state._visible = true;
		_state._hovered = ImGui::IsWindowHovered();
		_state._focused = ImGui::IsWindowFocused();
		_state._rectSize = { ImGui::GetWindowSize().x, ImGui::GetWindowSize().y };
		_state._activated = ImGui::IsWindowCollapsed();

		if (ImGui::GetWindowWidth() != (float)_width || ImGui::GetWindowHeight() != (float)_height)
		{
			_width = (int)ImGui::GetWindowWidth();
			_height = (int)ImGui::GetWindowHeight();
			_resized = true;
		}

		_width = (int)ImGui::GetWindowWidth();
		_height = (int)ImGui::GetWindowHeight();

		// update active window
		if (_state.isItemFocused())
		{

			float titleBarHeight = ImGui::GetStyle().FramePadding.y * 2 + ImGui::GetFontSize();

			// update mouse
			ImVec2 mousePos = ImGui::GetMousePos();
			float x = mousePos.x - ImGui::GetWindowPos().x;
			float y = mousePos.y - ImGui::GetWindowPos().y - titleBarHeight;
			mvInput::setMousePosition(x, y);

			if (mvApp::GetApp()->getItemRegistry().getActiveWindow() != _uuid)
				mvEventBus::Publish(mvEVT_CATEGORY_ITEM, mvEVT_ACTIVE_WINDOW, { CreateEventArgument("WINDOW", _uuid) });

		}

		_state.setPos({ ImGui::GetWindowPos().x , ImGui::GetWindowPos().y });

		if (_popup || _modal)
			ImGui::EndPopup();
		else
			ImGui::End();

		_collapsed = ImGui::IsWindowCollapsed();

		if (!_show)
		{
			_state._lastFrameUpdate = mvApp::s_frame;
			_state._hovered = false;
			_state._focused = false;
			_state._activated = false;
			_state._visible = false;

			if(_alias.empty())
				mvApp::GetApp()->getCallbackRegistry().addCallback(_on_close, _uuid, nullptr, _user_data);
			else
				mvApp::GetApp()->getCallbackRegistry().addCallback(_on_close, _alias, nullptr, _user_data);
		}

		if (_handlerRegistry)
			_handlerRegistry->customAction(&_state);
	}

	void mvWindowAppItem::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "modal"))
		{
			_modal = ToBool(item);
			_shownLastFrame = true;
		}

		if (PyObject* item = PyDict_GetItemString(dict, "popup"))
		{
			_popup = ToBool(item);
			_shownLastFrame = true;
		}

		if (PyObject* item = PyDict_GetItemString(dict, "label"))
		{
			_dirtyPos = true;
			_dirty_size = true;
		}

		if (PyObject* item = PyDict_GetItemString(dict, "no_close")) _no_close = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "collapsed"))
		{
			_collapsedDirty = true;
			_collapsed = ToBool(item);
		}

		if (PyObject* item = PyDict_GetItemString(dict, "min_size"))
		{
			auto min_size = ToIntVect(item);
			_min_size = { (float)min_size[0], (float)min_size[1]};
		}

		if (PyObject* item = PyDict_GetItemString(dict, "max_size"))
		{
			auto max_size = ToIntVect(item);
			_max_size = { (float)max_size[0], (float)max_size[1] };
		}

		if (PyObject* item = PyDict_GetItemString(dict, "on_close"))
		{
			if (_on_close)
				Py_XDECREF(_on_close);
			item = SanitizeCallback(item);
			if (item)
				Py_XINCREF(item);
			_on_close = item;
		}

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// window flags
		flagop("autosize", ImGuiWindowFlags_AlwaysAutoResize, _windowflags);
		flagop("no_move", ImGuiWindowFlags_NoMove, _windowflags);
		flagop("no_resize", ImGuiWindowFlags_NoResize, _windowflags);
		flagop("no_title_bar", ImGuiWindowFlags_NoTitleBar, _windowflags);
		flagop("no_scrollbar", ImGuiWindowFlags_NoScrollbar, _windowflags);
		flagop("no_collapse", ImGuiWindowFlags_NoCollapse, _windowflags);
		flagop("horizontal_scrollbar", ImGuiWindowFlags_HorizontalScrollbar, _windowflags);
		flagop("no_focus_on_appearing", ImGuiWindowFlags_NoFocusOnAppearing, _windowflags);
		flagop("no_bring_to_front_on_focus", ImGuiWindowFlags_NoBringToFrontOnFocus, _windowflags);
		flagop("menubar", ImGuiWindowFlags_MenuBar, _windowflags);
		flagop("no_background", ImGuiWindowFlags_NoBackground, _windowflags);
		flagop("no_saved_settings", ImGuiWindowFlags_NoSavedSettings, _windowflags);

		_oldxpos = _state.getItemPos().x;
		_oldypos = _state.getItemPos().y;
		_oldWidth = _width;
		_oldHeight = _height;
		_oldWindowflags = _windowflags;

	}

	void mvWindowAppItem::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "modal", mvPyObject(ToPyBool(_modal)));
		PyDict_SetItemString(dict, "popup", mvPyObject(ToPyBool(_popup)));
		PyDict_SetItemString(dict, "no_close", mvPyObject(ToPyBool(_no_close)));
		PyDict_SetItemString(dict, "collapsed", mvPyObject(ToPyBool(_collapsed)));
		PyDict_SetItemString(dict, "min_size", mvPyObject(ToPyPair(_min_size.x, _min_size.y)));
		PyDict_SetItemString(dict, "max_size", mvPyObject(ToPyPair(_max_size.x, _max_size.y)));

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, mvPyObject(ToPyBool(flags & flag)));
		};

		// window flags
		checkbitset("autosize", ImGuiWindowFlags_AlwaysAutoResize, _windowflags);
		checkbitset("no_resize", ImGuiWindowFlags_NoResize, _windowflags);
		checkbitset("no_title_bar", ImGuiWindowFlags_NoTitleBar, _windowflags);
		checkbitset("no_move", ImGuiWindowFlags_NoMove, _windowflags);
		checkbitset("no_scrollbar", ImGuiWindowFlags_NoScrollbar, _windowflags);
		checkbitset("no_collapse", ImGuiWindowFlags_NoCollapse, _windowflags);
		checkbitset("horizontal_scrollbar", ImGuiWindowFlags_HorizontalScrollbar, _windowflags);
		checkbitset("no_focus_on_appearing", ImGuiWindowFlags_NoFocusOnAppearing, _windowflags);
		checkbitset("no_bring_to_front_on_focus", ImGuiWindowFlags_NoBringToFrontOnFocus, _windowflags);
		checkbitset("menubar", ImGuiWindowFlags_MenuBar, _windowflags);
		checkbitset("no_background", ImGuiWindowFlags_NoBackground, _windowflags);
		checkbitset("no_saved_settings", ImGuiWindowFlags_NoSavedSettings, _windowflags);
	}

	PyObject* mvWindowAppItem::set_x_scroll(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw;
		float value;

		if (!(mvApp::GetApp()->getParsers())["set_x_scroll"].parse(args, kwargs, __FUNCTION__,
			&itemraw, &value))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);

		auto window = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (window == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "set_x_scroll",
				"Item not found: " + std::to_string(item), nullptr);
			return GetPyNone();
		}

		if (window->getType() == mvAppItemType::mvWindowAppItem)
		{

			auto pWindow = static_cast<mvWindowAppItem*>(window);

			pWindow->_scrollX = value;
			pWindow->_scrollXSet = true;
		}
		else if (window->getType() == mvAppItemType::mvChild)
		{
			auto pChild = static_cast<mvChild*>(window);

			pChild->setScrollX(value);
		}
		else
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "set_x_scroll",
				"Incompatible type. Expected types include: mvWindowAppItem, mvChild", window);
		}
		
		return GetPyNone();
	}

	PyObject* mvWindowAppItem::set_y_scroll(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw;
		float value;

		if (!(mvApp::GetApp()->getParsers())["set_y_scroll"].parse(args, kwargs, __FUNCTION__,
			&itemraw, &value))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);

		auto window = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (window == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "set_y_scroll",
				"Item not found: " + std::to_string(item), nullptr);
			return GetPyNone();
		}

		if (window->getType() == mvAppItemType::mvWindowAppItem)
		{

			auto pWindow = static_cast<mvWindowAppItem*>(window);

			pWindow->_scrollY = value;
			pWindow->_scrollYSet = true;
		}
		else if (window->getType() == mvAppItemType::mvChild)
		{
			auto pChild = static_cast<mvChild*>(window);

			pChild->setScrollY(value);
		}
		else
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "set_y_scroll",
				"Incompatible type. Expected types include: mvWindowAppItem, mvChild", window);
		}

		return GetPyNone();
	}

	PyObject* mvWindowAppItem::get_x_scroll(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw;

		if (!(mvApp::GetApp()->getParsers())["get_x_scroll"].parse(args, kwargs, __FUNCTION__,
			&itemraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);

		auto window = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (window == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "get_x_scroll",
				"Item not found: " + std::to_string(item), nullptr);
			return GetPyNone();
		}

		if (window->getType() == mvAppItemType::mvWindowAppItem)
		{

			auto pWindow = static_cast<mvWindowAppItem*>(window);

			return ToPyFloat(pWindow->_scrollX);
		}
		else if (window->getType() == mvAppItemType::mvChild)
		{
			auto pChild = static_cast<mvChild*>(window);

			return ToPyFloat(pChild->getScrollX());
		}
		else
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "get_x_scroll",
				"Incompatible type. Expected types include: mvWindowAppItem, mvChild", window);
		}

		return GetPyNone();
	}

	PyObject* mvWindowAppItem::get_y_scroll(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw;

		if (!(mvApp::GetApp()->getParsers())["get_y_scroll"].parse(args, kwargs, __FUNCTION__,
			&itemraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);

		auto window = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (window == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, s_command,
				"Item not found: " + std::to_string(item), nullptr);
			return GetPyNone();
		}

		if (window->getType() == mvAppItemType::mvWindowAppItem)
		{

			auto pWindow = static_cast<mvWindowAppItem*>(window);

			return ToPyFloat(pWindow->_scrollY);
		}
		else if (window->getType() == mvAppItemType::mvChild)
		{
			auto pChild = static_cast<mvChild*>(window);

			return ToPyFloat(pChild->getScrollY());
		}
		else
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "get_y_scroll",
				"Incompatible type. Expected types include: mvWindowAppItem, mvChild", window);
		}

		return GetPyNone();
	}

	PyObject* mvWindowAppItem::get_x_scroll_max(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw;

		if (!(mvApp::GetApp()->getParsers())["get_x_scroll_max"].parse(args, kwargs, __FUNCTION__,
			&itemraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);

		auto window = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (window == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, s_command,
				"Item not found: " + std::to_string(item), nullptr);
			return GetPyNone();
		}

		if (window->getType() == mvAppItemType::mvWindowAppItem)
		{

			auto pWindow = static_cast<mvWindowAppItem*>(window);

			return ToPyFloat(pWindow->_scrollMaxX);
		}
		else if (window->getType() == mvAppItemType::mvChild)
		{
			auto pChild = static_cast<mvChild*>(window);

			return ToPyFloat(pChild->getScrollXMax());
		}
		else
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "get_x_scroll_max",
				"Incompatible type. Expected types include: mvWindowAppItem, mvChild", window);
		}

		return GetPyNone();
	}

	PyObject* mvWindowAppItem::get_y_scroll_max(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw;

		if (!(mvApp::GetApp()->getParsers())["get_y_scroll_max"].parse(args, kwargs, __FUNCTION__,
			&itemraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);

		auto window = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (window == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, s_command,
				"Item not found: " + std::to_string(item), nullptr);
			return GetPyNone();
		}

		if (window->getType() == mvAppItemType::mvWindowAppItem)
		{

			auto pWindow = static_cast<mvWindowAppItem*>(window);

			return ToPyFloat(pWindow->_scrollMaxY);
		}
		else if (window->getType() == mvAppItemType::mvChild)
		{
			auto pChild = static_cast<mvChild*>(window);

			return ToPyFloat(pChild->getScrollYMax());
		}
		else
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleType, "set_y_scroll_max",
				"Incompatible type. Expected types include: mvWindowAppItem, mvChild", window);
		}

		return GetPyNone();
	}

}
