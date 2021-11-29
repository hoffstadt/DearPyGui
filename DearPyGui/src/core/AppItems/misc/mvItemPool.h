#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    //-----------------------------------------------------------------------------
    // mvItemSet
    //-----------------------------------------------------------------------------
    MV_REGISTER_WIDGET(mvItemSet);
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
    MV_REGISTER_WIDGET(mvItemPool);
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
