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

    MV_REGISTER_WIDGET(mvThemeComponent, MV_ITEM_DESC_DEFAULT | MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
    class mvThemeComponent : public mvAppItem
    {

        friend class mvTheme;

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvThemeComponent, add_theme_component)
        MV_NO_COMMANDS
        MV_NO_CONSTANTS

        MV_SET_STATES(MV_STATE_NONE);

        MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvTheme)
        MV_END_PARENTS

        MV_START_CHILDREN
            MV_ADD_CHILD(mvAppItemType::mvThemeColor),
            MV_ADD_CHILD(mvAppItemType::mvThemeStyle)
        MV_END_CHILDREN

    public:

        explicit mvThemeComponent(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificPositionalArgs(PyObject* dict) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void customAction(void* data = nullptr) override;
        void alternativeCustomAction(void* data = nullptr) override;

    private:

        int _specificType = (int)mvAppItemType::All;
        bool _specificEnabled = true;
        mvRef<mvAppItem>* _specificComponentPtr = nullptr;
        mvRef<mvAppItem> _oldComponent = nullptr;

    };

}
