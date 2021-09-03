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

#include "mvItemRegistry.h"

namespace Marvel {

    MV_REGISTER_WIDGET(mvChild, MV_ITEM_DESC_CONTAINER, StorageValueTypes::Bool, 1);
    class mvChild : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvChild, add_child)
        MV_NO_COMMANDS
        MV_DEFAULT_PARENTS
        MV_DEFAULT_CHILDREN
        MV_NO_CONSTANTS

        MV_SET_STATES(
            MV_STATE_HOVER |
            MV_STATE_ACTIVE |
            MV_STATE_FOCUSED |
            MV_STATE_DEACTIVATED |
            MV_STATE_RECT_SIZE |
            MV_STATE_CONT_AVAIL
        );

    public:

        explicit mvChild(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

        void addFlag(ImGuiWindowFlags flag);
        void removeFlag(ImGuiWindowFlags flag);

        void setScrollX(float value) { _scrollX = value; _scrollXSet = true; }
        void setScrollY(float value) { _scrollY = value; _scrollYSet = true; }
        float getScrollX() const { return _scrollX; }
        float getScrollY() const { return _scrollY; }
        float getScrollXMax() const { return _scrollMaxX; }
        float getScrollYMax() const { return _scrollMaxY; }

    private:

        bool             _border     = true;
        bool             _autosize_x = false;
        bool             _autosize_y = false;
        ImGuiWindowFlags _windowflags = ImGuiWindowFlags_NoSavedSettings;

        // scroll info
        float _scrollX = 0.0f;
        float _scrollY = 0.0f;
        float _scrollMaxX = 0.0f;
        float _scrollMaxY = 0.0f;
        bool  _scrollXSet = false;
        bool  _scrollYSet = false;

    };

}
