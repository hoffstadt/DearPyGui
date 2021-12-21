#include "mvChild.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"

namespace Marvel {

    mvChildWindow::mvChildWindow(mvUUID uuid)
        : mvAppItem(uuid)
    {
    }

    void mvChildWindow::applySpecificTemplate(mvAppItem* item)
    {
        auto titem = static_cast<mvChildWindow*>(item);
        _border = titem->_border;
        _autosize_x = titem->_autosize_x;
        _autosize_y = titem->_autosize_y;
        _windowflags = titem->_windowflags;
    }

    void mvChildWindow::addFlag(ImGuiWindowFlags flag)
    {
        _windowflags |= flag;
    }

    void mvChildWindow::removeFlag(ImGuiWindowFlags flag)
    {
        _windowflags &= ~flag;
    }

    void mvChildWindow::draw(ImDrawList* drawlist, float x, float y)
    {

        //-----------------------------------------------------------------------------
        // pre draw
        //-----------------------------------------------------------------------------

        // show/hide
        if (!_show)
            return;

        // focusing
        if (_focusNextFrame)
        {
            ImGui::SetKeyboardFocusHere();
            _focusNextFrame = false;
        }

        // cache old cursor position
        ImVec2 previousCursorPos = ImGui::GetCursorPos();

        // set cursor position if user set
        if (_dirtyPos)
            ImGui::SetCursorPos(_state.pos);

        // update widget's position state
        _state.pos = { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };

        // set item width
        if (_width != 0)
            ImGui::SetNextItemWidth((float)_width);

        // set indent
        if (_indent > 0.0f)
            ImGui::Indent(_indent);

        // push font if a font object is attached
        if (_font)
        {
            ImFont* fontptr = static_cast<mvFont*>(_font.get())->getFontPtr();
            ImGui::PushFont(fontptr);
        }

        // themes
        if (_enabled)
        {
            if (auto classTheme = GetClassThemeComponent(_type))
                static_cast<mvThemeComponent*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);
        }
        else
        {
            if (auto classTheme = GetDisabledClassThemeComponent(_type))
                static_cast<mvThemeComponent*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);
        }

        if (_theme)
        {
            static_cast<mvTheme*>(_theme.get())->setSpecificEnabled(_enabled);
            static_cast<mvTheme*>(_theme.get())->setSpecificType((int)_type);
            static_cast<mvTheme*>(_theme.get())->draw(nullptr, 0.0f, 0.0f);
        }

        //-----------------------------------------------------------------------------
        // draw
        //-----------------------------------------------------------------------------
        {
            ScopedID id(_uuid);

            ImGui::BeginChild(_internalLabel.c_str(), ImVec2(_autosize_x ? 0 : (float)_width, _autosize_y ? 0 : (float)_height), _border, _windowflags);
            _state.lastFrameUpdate = GContext->frame;
            _state.active = ImGui::IsItemActive();
            _state.deactivated = ImGui::IsItemDeactivated();
            _state.focused = ImGui::IsWindowFocused();
            _state.hovered = ImGui::IsWindowHovered();
            _state.rectSize = { ImGui::GetWindowWidth(), ImGui::GetWindowHeight() };
            _state.contextRegionAvail = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

            for (auto& item : _children[1])
            {

                item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
                if (item->_tracked)
                {
                    ImGui::SetScrollHereX(item->_trackOffset);
                    ImGui::SetScrollHereY(item->_trackOffset);
                }

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

            // allows this item to have a render callback
            if (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows))
            {

                // update mouse
                ImVec2 mousePos = ImGui::GetMousePos();
                float x = mousePos.x - ImGui::GetWindowPos().x;
                float y = mousePos.y - ImGui::GetWindowPos().y;
                GContext->input.mousePos.x = (int)x;
                GContext->input.mousePos.y = (int)y;

                if (GContext->itemRegistry->activeWindow != _uuid)
                    GContext->itemRegistry->activeWindow = _uuid;

            }

            _scrollX = ImGui::GetScrollX();
            _scrollY = ImGui::GetScrollY();
            _scrollMaxX = ImGui::GetScrollMaxX();
            _scrollMaxY = ImGui::GetScrollMaxY();

            ImGui::EndChild();
        }

        //-----------------------------------------------------------------------------
        // post draw
        //-----------------------------------------------------------------------------

        // set cursor position to cached position
        if (_dirtyPos)
            ImGui::SetCursorPos(previousCursorPos);

        if (_indent > 0.0f)
            ImGui::Unindent(_indent);

        // pop font off stack
        if (_font)
            ImGui::PopFont();

        // handle popping themes
        if (_enabled)
        {
            if (auto classTheme = GetClassThemeComponent(_type))
                static_cast<mvThemeComponent*>(classTheme.get())->customAction();
        }
        else
        {
            if (auto classTheme = GetDisabledClassThemeComponent(_type))
                static_cast<mvThemeComponent*>(classTheme.get())->customAction();
        }

        if (_theme)
        {
            static_cast<mvTheme*>(_theme.get())->setSpecificEnabled(_enabled);
            static_cast<mvTheme*>(_theme.get())->setSpecificType((int)_type);
            static_cast<mvTheme*>(_theme.get())->customAction();
        }

        if (_handlerRegistry)
            _handlerRegistry->customAction(&_state);

        // handle drag & drop payloads
        for (auto& item : _children[3])
            item->draw(nullptr, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

        // handle drag & drop if used
        if (_dropCallback)
        {
            ScopedID id(_uuid);
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(_payloadType.c_str()))
                {
                    auto payloadActual = static_cast<const mvDragPayload*>(payload->Data);
                    if (_alias.empty())
                        mvAddCallback(_dropCallback,_uuid, payloadActual->getDragData(), nullptr);
                    else
                        mvAddCallback(_dropCallback,_alias, payloadActual->getDragData(), nullptr);
                }

                ImGui::EndDragDropTarget();
            }
        }
    }

    void mvChildWindow::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;
         
        if (PyObject* item = PyDict_GetItemString(dict, "border")) _border = ToBool(item);
        if (PyObject* item = PyDict_GetItemString(dict, "autosize_x")) _autosize_x = ToBool(item);
        if (PyObject* item = PyDict_GetItemString(dict, "autosize_y")) _autosize_y = ToBool(item);

        // helper for bit flipping
        auto flagop = [dict](const char* keyword, int flag, int& flags)
        {
            if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
        };

        // window flags
        flagop("no_scrollbar", ImGuiWindowFlags_NoScrollbar, _windowflags);
        flagop("horizontal_scrollbar", ImGuiWindowFlags_HorizontalScrollbar, _windowflags);
        flagop("menubar", ImGuiWindowFlags_MenuBar, _windowflags);
    }

    void mvChildWindow::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;
         
        PyDict_SetItemString(dict, "border", mvPyObject(ToPyBool(_border)));
        PyDict_SetItemString(dict, "autosize_x", mvPyObject(ToPyBool(_autosize_x)));
        PyDict_SetItemString(dict, "autosize_y", mvPyObject(ToPyBool(_autosize_y)));

        // helper for bit flipping
        auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
        {
            PyDict_SetItemString(dict, keyword, mvPyObject(ToPyBool(flags & flag)));
        };

        // window flags
        checkbitset("no_scrollbar", ImGuiWindowFlags_NoScrollbar, _windowflags);
        checkbitset("horizontal_scrollbar", ImGuiWindowFlags_HorizontalScrollbar, _windowflags);
        checkbitset("menubar", ImGuiWindowFlags_MenuBar, _windowflags);
    }

}
