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

    MV_REGISTER_WIDGET(mvTabBar, MV_ITEM_DESC_CONTAINER, StorageValueTypes::UUID, 1);
    class mvTabBar : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTabBar, add_tab_bar)
        MV_NO_COMMANDS
        MV_DEFAULT_PARENTS
        MV_NO_CONSTANTS

        MV_SET_STATES(MV_STATE_VISIBLE);

        MV_START_CHILDREN
            MV_ADD_CHILD(mvAppItemType::mvTab),
            MV_ADD_CHILD(mvAppItemType::mvTabButton),
            MV_ADD_CHILD(mvAppItemType::mvActivatedHandler),
            MV_ADD_CHILD(mvAppItemType::mvActiveHandler),
            MV_ADD_CHILD(mvAppItemType::mvClickedHandler),
            MV_ADD_CHILD(mvAppItemType::mvDeactivatedAfterEditHandler),
            MV_ADD_CHILD(mvAppItemType::mvDeactivatedHandler),
            MV_ADD_CHILD(mvAppItemType::mvEditedHandler),
            MV_ADD_CHILD(mvAppItemType::mvFocusHandler),
            MV_ADD_CHILD(mvAppItemType::mvHoverHandler),
            MV_ADD_CHILD(mvAppItemType::mvResizeHandler),
            MV_ADD_CHILD(mvAppItemType::mvToggledOpenHandler),
            MV_ADD_CHILD(mvAppItemType::mvVisibleHandler)
        MV_END_CHILDREN

    public:

        explicit mvTabBar(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

        // values
        void setDataSource(mvUUID dataSource) override;
        void* getValue() override { return &_value; }
        PyObject* getPyValue() override;
        void setPyValue(PyObject* value) override;

        mvUUID getSpecificValue();
        void setValue(mvUUID value);

    private:
        
        mvRef<mvUUID>    _value = CreateRef<mvUUID>(0);
        mvUUID           _disabled_value = 0;
        ImGuiTabBarFlags _flags = ImGuiTabBarFlags_None;
        mvUUID           _lastValue = 0;
        mvUUID           _uiValue = 0; // value suggested from UI

    };

}
