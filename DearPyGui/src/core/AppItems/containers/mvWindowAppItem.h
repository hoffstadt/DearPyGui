/***************************************************************************//*/
Copyright (c) 2021 Dear PyGui, LLC

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

/******************************************************************************/

#pragma once

#include <utility>
#include "mvItemRegistry.h"
#include "mvContext.h"

namespace Marvel {

    MV_REGISTER_WIDGET(mvWindowAppItem, MV_ITEM_DESC_CONTAINER | MV_ITEM_DESC_ROOT, StorageValueTypes::None, 1);
    class mvWindowAppItem : public mvAppItem
    {

        friend class mvResizeHandler;

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvWindowAppItem, add_window)
        MV_DEFAULT_PARENTS
        MV_DEFAULT_CHILDREN
        MV_NO_CONSTANTS

        MV_CREATE_COMMAND(set_x_scroll);
        MV_CREATE_COMMAND(set_y_scroll);
        MV_CREATE_COMMAND(get_x_scroll);
        MV_CREATE_COMMAND(get_y_scroll);
        MV_CREATE_COMMAND(get_x_scroll_max);
        MV_CREATE_COMMAND(get_y_scroll_max);

        MV_SET_STATES(
            MV_STATE_HOVER |
            MV_STATE_FOCUSED | 
            MV_STATE_VISIBLE | 
            MV_STATE_RECT_SIZE |
            MV_STATE_TOGGLED_OPEN);

        MV_START_COMMANDS
            MV_ADD_COMMAND(set_x_scroll);
            MV_ADD_COMMAND(set_y_scroll);
            MV_ADD_COMMAND(get_x_scroll);
            MV_ADD_COMMAND(get_y_scroll);
            MV_ADD_COMMAND(get_x_scroll_max);
            MV_ADD_COMMAND(get_y_scroll_max)
        MV_END_COMMANDS

        enum class Status{ Normal, Transition, Dirty};

    public:

        explicit mvWindowAppItem(mvUUID uuid, bool mainWindow = false);

        void addFlag              (ImGuiWindowFlags flag) { _windowflags |= flag; }
        void removeFlag           (ImGuiWindowFlags flag) { _windowflags &= ~flag; }
        void setWindowAsMainStatus(bool value);
        void draw                 (ImDrawList* drawlist, float x, float y) override;
        bool getWindowAsMainStatus() const { return _mainWindow; }
        void onChildAdd(mvRef<mvAppItem> item) override;
        void onChildRemoved(mvRef<mvAppItem> item) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

        ~mvWindowAppItem();

    private:

        ImGuiWindowFlags _windowflags = ImGuiWindowFlags_None;
        ImGuiWindowFlags _oldWindowflags = ImGuiWindowFlags_None;
        float            _oldxpos = 200;
        float            _oldypos = 200;
        int              _oldWidth = 200;
        int              _oldHeight = 200;
        bool             _mainWindow = false;
        bool             _closing = true;
        bool             _collapsedDirty = true;
        bool             _resized = false;
        bool             _modal = false;
        bool             _popup = false;
        bool             _autosize = false;
        bool             _no_resize = false;
        bool             _no_title_bar = false;
        bool             _no_move = false;
        bool             _no_scrollbar = false;
        bool             _no_collapse = false;
        bool             _horizontal_scrollbar = false;
        bool             _no_focus_on_appearing = false;
        bool             _no_bring_to_front_on_focus = false;
        bool             _menubar = false;
        bool             _no_close = false;
        bool             _no_background = false;
        bool             _collapsed = false;
        PyObject*        _on_close = nullptr;
        mvVec2           _min_size = { 100.0f, 100.0f };
        mvVec2           _max_size = { 30000.0f, 30000.0f };

        // scroll info
        float _scrollX = 0.0f;
        float _scrollY = 0.0f;
        float _scrollMaxX = 0.0f;
        float _scrollMaxY = 0.0f;
        bool  _scrollXSet = false;
        bool  _scrollYSet = false;
        
    };

}
