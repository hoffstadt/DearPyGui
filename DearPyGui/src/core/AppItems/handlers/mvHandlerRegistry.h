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

    MV_REGISTER_WIDGET(mvHandlerRegistry, MV_ITEM_DESC_ROOT | MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
    class mvHandlerRegistry : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvHandlerRegistry, add_handler_registry)
        MV_NO_COMMANDS
        MV_DEFAULT_PARENTS
        MV_NO_CONSTANTS

        MV_SET_STATES(MV_STATE_NONE);

        MV_START_CHILDREN
            MV_ADD_CHILD(mvAppItemType::mvKeyDownHandler),
            MV_ADD_CHILD(mvAppItemType::mvKeyPressHandler),
            MV_ADD_CHILD(mvAppItemType::mvKeyReleaseHandler),
            MV_ADD_CHILD(mvAppItemType::mvMouseMoveHandler),
            MV_ADD_CHILD(mvAppItemType::mvMouseWheelHandler),
            MV_ADD_CHILD(mvAppItemType::mvMouseClickHandler),
            MV_ADD_CHILD(mvAppItemType::mvMouseDoubleClickHandler),
            MV_ADD_CHILD(mvAppItemType::mvMouseDownHandler),
            MV_ADD_CHILD(mvAppItemType::mvMouseReleaseHandler),
            MV_ADD_CHILD(mvAppItemType::mvMouseDragHandler)
        MV_END_CHILDREN

    public:

        explicit mvHandlerRegistry(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;

    };

}
