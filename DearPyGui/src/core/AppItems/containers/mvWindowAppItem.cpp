#include "mvWindowAppItem.h"
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
            std::vector<mvPythonDataElement> args;

            AddCommonArgs(args,(CommonParserArgs)(
                MV_PARSER_ARG_ID |
                MV_PARSER_ARG_WIDTH |
                MV_PARSER_ARG_HEIGHT |
                MV_PARSER_ARG_INDENT |
                MV_PARSER_ARG_SEARCH_DELAY |
                MV_PARSER_ARG_SHOW |
                MV_PARSER_ARG_POS)
            );

            args.push_back({ mvPyDataType::IntList, "min_size", mvArgType::KEYWORD_ARG, "[100, 100]", "Minimum window size." });
            args.push_back({ mvPyDataType::IntList, "max_size", mvArgType::KEYWORD_ARG, "[30000, 30000]", "Maximum window size." });
            args.push_back({ mvPyDataType::Bool, "menubar", mvArgType::KEYWORD_ARG, "False", "Shows or hides the menubar." });
            args.push_back({ mvPyDataType::Bool, "collapsed", mvArgType::KEYWORD_ARG, "False", "Collapse the window." });
            args.push_back({ mvPyDataType::Bool, "autosize", mvArgType::KEYWORD_ARG, "False", "Autosized the window to fit it's items." });
            args.push_back({ mvPyDataType::Bool, "no_resize", mvArgType::KEYWORD_ARG, "False", "Allows for the window size to be changed or fixed." });
            args.push_back({ mvPyDataType::Bool, "no_title_bar", mvArgType::KEYWORD_ARG, "False", "Title name for the title bar of the window." });
            args.push_back({ mvPyDataType::Bool, "no_move", mvArgType::KEYWORD_ARG, "False", "Allows for the window's position to be changed or fixed." });
            args.push_back({ mvPyDataType::Bool, "no_scrollbar", mvArgType::KEYWORD_ARG, "False", " Disable scrollbars. (window can still scroll with mouse or programmatically)" });
            args.push_back({ mvPyDataType::Bool, "no_collapse", mvArgType::KEYWORD_ARG, "False", "Disable user collapsing window by double-clicking on it." });
            args.push_back({ mvPyDataType::Bool, "horizontal_scrollbar", mvArgType::KEYWORD_ARG, "False", "Allow horizontal scrollbar to appear. (off by default)" });
            args.push_back({ mvPyDataType::Bool, "no_focus_on_appearing", mvArgType::KEYWORD_ARG, "False", "Disable taking focus when transitioning from hidden to visible state." });
            args.push_back({ mvPyDataType::Bool, "no_bring_to_front_on_focus", mvArgType::KEYWORD_ARG, "False", "Disable bringing window to front when taking focus. (e.g. clicking on it or programmatically giving it focus)" });
            args.push_back({ mvPyDataType::Bool, "no_close", mvArgType::KEYWORD_ARG, "False", "Disable user closing the window by removing the close button." });
            args.push_back({ mvPyDataType::Bool, "no_background", mvArgType::KEYWORD_ARG, "False", "Sets Background and border alpha to transparent." });
            args.push_back({ mvPyDataType::Bool, "modal", mvArgType::KEYWORD_ARG, "False", "Fills area behind window according to the theme and disables user ability to interact with anything except the window." });
            args.push_back({ mvPyDataType::Bool, "popup", mvArgType::KEYWORD_ARG, "False", "Fills area behind window according to the theme, removes title bar, collapse and close. Window can be closed by selecting area in the background behind the window." });
            args.push_back({ mvPyDataType::Bool, "no_saved_settings", mvArgType::KEYWORD_ARG, "False", "Never load/save settings in .ini file." });
            
            args.push_back({ mvPyDataType::Callable, "on_close", mvArgType::KEYWORD_ARG, "None", "Callback ran when window is closed." });

            mvPythonParserSetup setup;
            setup.about = "Creates a new window for following items to be added to.";
            setup.category = { "Containers", "Widgets" };
            setup.returnType = mvPyDataType::UUID;
            setup.createContextManager = true;

            mvPythonParser parser = FinalizeParser(setup, args);

            parsers->insert({ s_command, parser });
        }

        {
            std::vector<mvPythonDataElement> args;
            args.push_back({ mvPyDataType::UUID, "item" });
            args.push_back({ mvPyDataType::Float, "value" });

            mvPythonParserSetup setup;

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "set_x_scroll", parser });
        }

        {
            std::vector<mvPythonDataElement> args;
            args.push_back({ mvPyDataType::UUID, "item" });
            args.push_back({ mvPyDataType::Float, "value" });
            mvPythonParserSetup setup;
            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "set_y_scroll", parser });
        }

        {
            std::vector<mvPythonDataElement> args;
            args.push_back({ mvPyDataType::UUID , "item" });
            mvPythonParserSetup setup;
            setup.returnType = mvPyDataType::Float;
            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "get_x_scroll", parser });
        }

        {
            std::vector<mvPythonDataElement> args;
            args.push_back({ mvPyDataType::UUID, "item" });
            mvPythonParserSetup setup;
            setup.returnType = mvPyDataType::Float;
            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "get_y_scroll", parser });
        }

        {
            std::vector<mvPythonDataElement> args;
            args.push_back({ mvPyDataType::UUID , "item" });
            mvPythonParserSetup setup;
            setup.returnType = mvPyDataType::Float;
            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "get_x_scroll_max", parser });
        }

        {
            std::vector<mvPythonDataElement> args;
            args.push_back({ mvPyDataType::UUID, "item" });
            mvPythonParserSetup setup;
            setup.returnType = mvPyDataType::Float;
            mvPythonParser parser = FinalizeParser(setup, args);
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
        mvSubmitCallback([callback]() {
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
            _oldxpos = _state.pos.x;
            _oldypos = _state.pos.y;
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
            _state.pos = { _oldxpos , _oldypos };
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

        if (GContext->frame == 1 && !GContext->IO.iniFile.empty() && !(_windowflags & ImGuiWindowFlags_NoSavedSettings))
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

        // themes
        if (_enabled)
        {
            if (auto classTheme = getClassThemeComponent())
                static_cast<mvThemeComponent*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);
        }
        else
        {
            if (auto classTheme = getClassDisabledThemeComponent())
                static_cast<mvThemeComponent*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);
        }

        if (_theme)
        {
            static_cast<mvTheme*>(_theme.get())->setSpecificEnabled(_enabled);
            static_cast<mvTheme*>(_theme.get())->setSpecificType((int)getType());
            static_cast<mvTheme*>(_theme.get())->draw(nullptr, 0.0f, 0.0f);
        }


        //-----------------------------------------------------------------------------
        // draw
        //-----------------------------------------------------------------------------

        ScopedID id(_uuid);

        if (_mainWindow)
        {
            ImGui::SetNextWindowBgAlpha(1.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f); // to prevent main window corners from showing
            ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
            ImGui::SetNextWindowSize(ImVec2((float)GContext->viewport->clientWidth, (float)GContext->viewport->clientHeight));
        }

        else if (_dirtyPos)
        {
            ImGui::SetNextWindowPos(_state.pos);
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
            if (_shownLastFrame)
            {
                _shownLastFrame = false;
                ImGui::OpenPopup(_internalLabel.c_str(), ImGuiPopupFlags_NoOpenOverExistingPopup);
            }
            
            if (!ImGui::BeginPopupModal(_internalLabel.c_str(), _no_close ? nullptr : &_show, _windowflags))
            {
                if (_mainWindow)
                    ImGui::PopStyleVar();

                // shouldn't have to do this but do. Fix later
                _show = false;
                _state.lastFrameUpdate = GContext->frame;
                _state.hovered = false;
                _state.focused = false;
                _state.toggledOpen = false;
                _state.visible = false;

                if(_alias.empty())
                    mvAddCallback(_on_close, _uuid, nullptr, _user_data);
                else
                    mvAddCallback(_on_close, _alias, nullptr, _user_data);
                
                return;
            }
        }

        else if (_popup)
        {
            if (_shownLastFrame)
            {
                _shownLastFrame = false;
                ImGui::OpenPopup(_internalLabel.c_str(), ImGuiPopupFlags_NoOpenOverExistingPopup);
            }

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
            if (!item->_show)
                continue;

            item->draw(this_drawlist, startx, starty);

            UpdateAppItemState(item->_state);

        }

        for (auto& item : _children[1])
        {

            item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
            if (item->_tracked)
                ImGui::SetScrollHereY(item->_trackOffset);

        }

        for (auto& item : _children[2])
        {
            // skip item if it's not shown
            if (!item->_show)
                continue;

            item->draw(this_drawlist, startx, starty);

            UpdateAppItemState(item->_state);

        }

        //-----------------------------------------------------------------------------
        // post draw
        //-----------------------------------------------------------------------------

        // pop font from stack
        if (_font)
            ImGui::PopFont();

        // handle popping themes
        if (_enabled)
        {
            if (auto classTheme = getClassThemeComponent())
                static_cast<mvThemeComponent*>(classTheme.get())->customAction();
        }
        else
        {
            if (auto classTheme = getClassDisabledThemeComponent())
                static_cast<mvThemeComponent*>(classTheme.get())->customAction();
        }

        if (_theme)
        {
            static_cast<mvTheme*>(_theme.get())->setSpecificEnabled(_enabled);
            static_cast<mvTheme*>(_theme.get())->setSpecificType((int)getType());
            static_cast<mvTheme*>(_theme.get())->customAction();
        }

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

        //-----------------------------------------------------------------------------
        // update state
        //-----------------------------------------------------------------------------

        _state.lastFrameUpdate = GContext->frame;
        _state.visible = true;
        _state.hovered = ImGui::IsWindowHovered();
        _state.focused = ImGui::IsWindowFocused();
        _state.rectSize = { ImGui::GetWindowSize().x, ImGui::GetWindowSize().y };
        _state.toggledOpen = ImGui::IsWindowCollapsed();
        if (_state.mvPrevRectSize.x != _state.rectSize.x || _state.mvPrevRectSize.y != _state.rectSize.y) { _state.mvRectSizeResized = true; }
        else _state.mvRectSizeResized = false;
        _state.mvPrevRectSize = _state.rectSize;

        if (ImGui::GetWindowWidth() != (float)_width || ImGui::GetWindowHeight() != (float)_height)
        {
            _width = (int)ImGui::GetWindowWidth();
            _height = (int)ImGui::GetWindowHeight();
            _resized = true;
        }

        _width = (int)ImGui::GetWindowWidth();
        _height = (int)ImGui::GetWindowHeight();

        // update active window
        if (IsItemFocused(_state))
        {

            float titleBarHeight = ImGui::GetStyle().FramePadding.y * 2 + ImGui::GetFontSize();

            // update mouse
            ImVec2 mousePos = ImGui::GetMousePos();
            float x = mousePos.x - ImGui::GetWindowPos().x;
            float y = mousePos.y - ImGui::GetWindowPos().y - titleBarHeight;
            GContext->input.mousePos.x = (int)x;
            GContext->input.mousePos.y = (int)y;

            if (GContext->itemRegistry->activeWindow != _uuid)
                GContext->itemRegistry->activeWindow = _uuid;

        }

        _state.pos = { ImGui::GetWindowPos().x , ImGui::GetWindowPos().y };

        if (_popup || _modal)
            ImGui::EndPopup();
        else
            ImGui::End();

        _collapsed = ImGui::IsWindowCollapsed();

        if (!_show)
        {
            _state.lastFrameUpdate = GContext->frame;
            _state.hovered = false;
            _state.focused = false;
            _state.toggledOpen = false;
            _state.visible = false;

            if(_alias.empty())
                mvAddCallback(_on_close, _uuid, nullptr, _user_data);
            else
                mvAddCallback(_on_close, _alias, nullptr, _user_data);
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

        _oldxpos = _state.pos.x;
        _oldypos = _state.pos.y;
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

        if (!Parse((GetParsers())["set_x_scroll"], args, kwargs, __FUNCTION__,
            &itemraw, &value))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID item = GetIDFromPyObject(itemraw);

        auto window = GetItem((*GContext->itemRegistry), item);
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
        else if (window->getType() == mvAppItemType::mvChildWindow)
        {
            auto pChild = static_cast<mvChildWindow*>(window);

            pChild->setScrollX(value);
        }
        else
        {
            mvThrowPythonError(mvErrorCode::mvIncompatibleType, "set_x_scroll",
                "Incompatible type. Expected types include: mvWindowAppItem, mvChildWindow", window);
        }
        
        return GetPyNone();
    }

    PyObject* mvWindowAppItem::set_y_scroll(PyObject* self, PyObject* args, PyObject* kwargs)
    {

        PyObject* itemraw;
        float value;

        if (!Parse((GetParsers())["set_y_scroll"], args, kwargs, __FUNCTION__,
            &itemraw, &value))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID item = GetIDFromPyObject(itemraw);

        auto window = GetItem((*GContext->itemRegistry), item);
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
        else if (window->getType() == mvAppItemType::mvChildWindow)
        {
            auto pChild = static_cast<mvChildWindow*>(window);

            pChild->setScrollY(value);
        }
        else
        {
            mvThrowPythonError(mvErrorCode::mvIncompatibleType, "set_y_scroll",
                "Incompatible type. Expected types include: mvWindowAppItem, mvChildWindow", window);
        }

        return GetPyNone();
    }

    PyObject* mvWindowAppItem::get_x_scroll(PyObject* self, PyObject* args, PyObject* kwargs)
    {

        PyObject* itemraw;

        if (!Parse((GetParsers())["get_x_scroll"], args, kwargs, __FUNCTION__,
            &itemraw))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID item = GetIDFromPyObject(itemraw);

        auto window = GetItem((*GContext->itemRegistry), item);
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
        else if (window->getType() == mvAppItemType::mvChildWindow)
        {
            auto pChild = static_cast<mvChildWindow*>(window);

            return ToPyFloat(pChild->getScrollX());
        }
        else
        {
            mvThrowPythonError(mvErrorCode::mvIncompatibleType, "get_x_scroll",
                "Incompatible type. Expected types include: mvWindowAppItem, mvChildWindow", window);
        }

        return GetPyNone();
    }

    PyObject* mvWindowAppItem::get_y_scroll(PyObject* self, PyObject* args, PyObject* kwargs)
    {

        PyObject* itemraw;

        if (!Parse((GetParsers())["get_y_scroll"], args, kwargs, __FUNCTION__,
            &itemraw))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID item = GetIDFromPyObject(itemraw);

        auto window = GetItem((*GContext->itemRegistry), item);
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
        else if (window->getType() == mvAppItemType::mvChildWindow)
        {
            auto pChild = static_cast<mvChildWindow*>(window);

            return ToPyFloat(pChild->getScrollY());
        }
        else
        {
            mvThrowPythonError(mvErrorCode::mvIncompatibleType, "get_y_scroll",
                "Incompatible type. Expected types include: mvWindowAppItem, mvChildWindow", window);
        }

        return GetPyNone();
    }

    PyObject* mvWindowAppItem::get_x_scroll_max(PyObject* self, PyObject* args, PyObject* kwargs)
    {

        PyObject* itemraw;

        if (!Parse((GetParsers())["get_x_scroll_max"], args, kwargs, __FUNCTION__,
            &itemraw))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID item = GetIDFromPyObject(itemraw);

        auto window = GetItem((*GContext->itemRegistry), item);
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
        else if (window->getType() == mvAppItemType::mvChildWindow)
        {
            auto pChild = static_cast<mvChildWindow*>(window);

            return ToPyFloat(pChild->getScrollXMax());
        }
        else
        {
            mvThrowPythonError(mvErrorCode::mvIncompatibleType, "get_x_scroll_max",
                "Incompatible type. Expected types include: mvWindowAppItem, mvChildWindow", window);
        }

        return GetPyNone();
    }

    PyObject* mvWindowAppItem::get_y_scroll_max(PyObject* self, PyObject* args, PyObject* kwargs)
    {

        PyObject* itemraw;

        if (!Parse((GetParsers())["get_y_scroll_max"], args, kwargs, __FUNCTION__,
            &itemraw))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID item = GetIDFromPyObject(itemraw);

        auto window = GetItem((*GContext->itemRegistry), item);
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
        else if (window->getType() == mvAppItemType::mvChildWindow)
        {
            auto pChild = static_cast<mvChildWindow*>(window);

            return ToPyFloat(pChild->getScrollYMax());
        }
        else
        {
            mvThrowPythonError(mvErrorCode::mvIncompatibleType, "set_y_scroll_max",
                "Incompatible type. Expected types include: mvWindowAppItem, mvChildWindow", window);
        }

        return GetPyNone();
    }

}
