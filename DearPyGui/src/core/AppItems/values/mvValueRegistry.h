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

#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include "mvModule_DearPyGui.h"
#include "cpp.hint"

namespace Marvel {

    MV_REGISTER_WIDGET(mvValueRegistry, MV_ITEM_DESC_ROOT | MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
    class mvValueRegistry : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvValueRegistry, add_value_registry)
        MV_NO_COMMANDS
        MV_DEFAULT_PARENTS
        MV_NO_CONSTANTS

        MV_SET_STATES(MV_STATE_NONE);

        MV_START_CHILDREN
            MV_ADD_CHILD(mvAppItemType::mvBoolValue),
            MV_ADD_CHILD(mvAppItemType::mvIntValue),
            MV_ADD_CHILD(mvAppItemType::mvInt4Value),
            MV_ADD_CHILD(mvAppItemType::mvFloatValue),
            MV_ADD_CHILD(mvAppItemType::mvFloat4Value),
            MV_ADD_CHILD(mvAppItemType::mvStringValue),
            MV_ADD_CHILD(mvAppItemType::mvDoubleValue),
            MV_ADD_CHILD(mvAppItemType::mvDouble4Value),
            MV_ADD_CHILD(mvAppItemType::mvColorValue),
            MV_ADD_CHILD(mvAppItemType::mvFloatVectValue),
            MV_ADD_CHILD(mvAppItemType::mvSeriesValue)
        MV_END_CHILDREN

    public:

        explicit mvValueRegistry(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override {}

    };

}
