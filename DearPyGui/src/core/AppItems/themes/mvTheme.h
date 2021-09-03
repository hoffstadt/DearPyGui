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

    MV_REGISTER_WIDGET(mvTheme, MV_ITEM_DESC_DEFAULT | MV_ITEM_DESC_ROOT | MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
    class mvTheme : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTheme, add_theme)
        MV_DEFAULT_PARENTS

        MV_CREATE_COMMAND(bind_theme);

        MV_SET_STATES(MV_STATE_NONE);

        MV_START_COMMANDS
            MV_ADD_COMMAND(bind_theme);
        MV_END_COMMANDS

        MV_START_CHILDREN
            MV_ADD_CHILD(mvAppItemType::mvThemeColor),
            MV_ADD_CHILD(mvAppItemType::mvThemeStyle)
        MV_END_CHILDREN

        MV_CREATE_CONSTANT(mvThemeCat_Core, 0);
        MV_CREATE_CONSTANT(mvThemeCat_Plots, 1);
        MV_CREATE_CONSTANT(mvThemeCat_Nodes, 2);

        MV_START_CONSTANTS
            MV_ADD_CONSTANT(mvThemeCat_Core),
            MV_ADD_CONSTANT(mvThemeCat_Plots),
            MV_ADD_CONSTANT(mvThemeCat_Nodes),
        MV_END_CONSTANTS

    public:

        explicit mvTheme(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void customAction(void* data = nullptr) override;
        void alternativeCustomAction(void* data = nullptr) override;

    private:

        bool _default_theme = false;

    };

}
