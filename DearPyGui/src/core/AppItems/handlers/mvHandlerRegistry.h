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

        MV_START_COMMANDS
        MV_END_COMMANDS

        MV_START_CONSTANTS
        MV_END_CONSTANTS

    public:

        mvHandlerRegistry(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;

        bool canChildBeAdded(mvAppItemType type) override;

    };

}
