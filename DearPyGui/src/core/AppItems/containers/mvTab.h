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

    MV_REGISTER_WIDGET(mvTab, MV_ITEM_DESC_CONTAINER, StorageValueTypes::Bool, 1);
    class mvTab : public mvAppItem
    {

        enum class TabOrdering {
            mvTabOrder_Reorderable = 0L,
            mvTabOrder_Fixed,
            mvTabOrder_Leading,
            mvTabOrder_Trailing
        };

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTab, add_tab)
        MV_NO_COMMANDS
        MV_DEFAULT_CHILDREN

        MV_CREATE_CONSTANT(mvTabOrder_Reorderable, 0L);
        MV_CREATE_CONSTANT(mvTabOrder_Fixed, 1L);
        MV_CREATE_CONSTANT(mvTabOrder_Leading, 2L);
        MV_CREATE_CONSTANT(mvTabOrder_Trailing, 3L);

        MV_SET_STATES(
            MV_STATE_HOVER |
            MV_STATE_ACTIVE |
            MV_STATE_CLICKED |
            MV_STATE_VISIBLE |
            MV_STATE_ACTIVATED |
            MV_STATE_DEACTIVATED |
            MV_STATE_RECT_MIN |
            MV_STATE_RECT_MAX |
            MV_STATE_RECT_SIZE |
            MV_STATE_CONT_AVAIL
        );


        MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvTabBar),
            MV_ADD_PARENT(mvAppItemType::mvStage),
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
        MV_END_PARENTS

        MV_START_CONSTANTS
            MV_ADD_CONSTANT(mvTabOrder_Reorderable),
            MV_ADD_CONSTANT(mvTabOrder_Fixed),
            MV_ADD_CONSTANT(mvTabOrder_Leading),
            MV_ADD_CONSTANT(mvTabOrder_Trailing)
        MV_END_CONSTANTS

    public:

        explicit mvTab(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

        // values
        void setDataSource(mvUUID dataSource) override;
        void* getValue() override { return &_value; }
        PyObject* getPyValue() override;
        void setPyValue(PyObject* value) override;
        
        void addFlag   (ImGuiTabItemFlags flag);
        void removeFlag(ImGuiTabItemFlags flag);

    private:

        mvRef<bool>       _value = CreateRef<bool>(false);
        bool              _disabled_value = false;
        bool              _closable = false;
        ImGuiTabItemFlags _flags = ImGuiTabItemFlags_None;

    };

}
