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

    //-----------------------------------------------------------------------------
    // mvItemSet
    //-----------------------------------------------------------------------------
    MV_REGISTER_WIDGET(mvItemSet, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 1);
    class mvItemSet : public mvAppItem
    {

        friend class mvItemPool;

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvItemSet, add_item_set)
        MV_NO_COMMANDS
        MV_NO_CONSTANTS
        MV_DEFAULT_CHILDREN

        MV_SET_STATES(MV_STATE_NONE);

        MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvItemPool)
        MV_END_PARENTS

    public:

        explicit mvItemSet(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override {}
        void handleSpecificRequiredArgs(PyObject* args) override;
        void getSpecificConfiguration(PyObject* dict) override;

        mvRef<mvAppItem> getItem();
        void returnItem(mvRef<mvAppItem> item);

    private:

        // config
        int _itemType = -1;
        int _itemCount = 0;

        std::vector<mvRef<mvAppItem>> _availableItems;
        std::vector<mvRef<mvAppItem>> _returnedItems;

        // internal
        int _currentAvailableIndex = 0;
        int _currentReturnIndex = 0;
    };

    //-----------------------------------------------------------------------------
    // mvItemPool
    //-----------------------------------------------------------------------------
    MV_REGISTER_WIDGET(mvItemPool, MV_ITEM_DESC_ROOT | MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
    class mvItemPool : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvItemPool, add_item_pool)
        MV_NO_COMMANDS
        MV_DEFAULT_PARENTS
        MV_NO_CONSTANTS

        MV_SET_STATES(MV_STATE_NONE);

        MV_START_CHILDREN
            MV_ADD_CHILD(mvAppItemType::mvItemSet)
        MV_END_CHILDREN

    public:

        mvItemPool(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override {}

        mvRef<mvAppItem> getItem(mvAppItemType itemType);
        void returnItem(mvRef<mvAppItem> item);

    };

}
