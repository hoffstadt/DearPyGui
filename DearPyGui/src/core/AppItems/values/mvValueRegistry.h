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

        MV_START_COMMANDS
        MV_END_COMMANDS

        MV_START_CONSTANTS
        MV_END_CONSTANTS

    public:

        mvValueRegistry(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override {}

        bool canChildBeAdded(mvAppItemType type) override;

    };

}
