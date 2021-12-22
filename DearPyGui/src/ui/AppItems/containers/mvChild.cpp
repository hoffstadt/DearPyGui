#include "mvChild.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"
#include "AppItems/widget_handlers/mvItemHandlerRegistry.h"

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
        if (!config.show)
            return;

        // focusing
        if (info.focusNextFrame)
        {
            ImGui::SetKeyboardFocusHere();
            info.focusNextFrame = false;
        }

        // cache old cursor position
        ImVec2 previousCursorPos = ImGui::GetCursorPos();

        // set cursor position if user set
        if (info.dirtyPos)
            ImGui::SetCursorPos(state.pos);

        // update widget's position state
        state.pos = { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };

        // set item width
        if (config.width != 0)
            ImGui::SetNextItemWidth((float)config.width);

        // set indent
        if (config.indent > 0.0f)
            ImGui::Indent(config.indent);

        // push font if a font object is attached
        if (font)
        {
            ImFont* fontptr = static_cast<mvFont*>(font.get())->getFontPtr();
            ImGui::PushFont(fontptr);
        }

        // themes
        apply_local_theming(this);

        //-----------------------------------------------------------------------------
        // draw
        //-----------------------------------------------------------------------------
        {
            ScopedID id(uuid);

            ImGui::BeginChild(info.internalLabel.c_str(), ImVec2(_autosize_x ? 0 : (float)config.width, _autosize_y ? 0 : (float)config.height), _border, _windowflags);
            state.lastFrameUpdate = GContext->frame;
            state.active = ImGui::IsItemActive();
            state.deactivated = ImGui::IsItemDeactivated();
            state.focused = ImGui::IsWindowFocused();
            state.hovered = ImGui::IsWindowHovered();
            state.rectSize = { ImGui::GetWindowWidth(), ImGui::GetWindowHeight() };
            state.contextRegionAvail = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };

            for (auto& item : childslots[1])
            {

                item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
                if (item->config.tracked)
                {
                    ImGui::SetScrollHereX(item->config.trackOffset);
                    ImGui::SetScrollHereY(item->config.trackOffset);
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

                if (GContext->itemRegistry->activeWindow != uuid)
                    GContext->itemRegistry->activeWindow = uuid;

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
        if (info.dirtyPos)
            ImGui::SetCursorPos(previousCursorPos);

        if (config.indent > 0.0f)
            ImGui::Unindent(config.indent);

        // pop font off stack
        if (font)
            ImGui::PopFont();

        // handle popping themes
        cleanup_local_theming(this);

        if (handlerRegistry)
            handlerRegistry->checkEvents(&state);

        // handle drag & drop if used
        apply_drag_drop(this);
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
