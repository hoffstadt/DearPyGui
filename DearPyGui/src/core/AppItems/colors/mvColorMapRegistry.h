#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include "mvModule_DearPyGui.h"
#include "cpp.hint"

namespace Marvel {

    MV_REGISTER_WIDGET(mvColorMapRegistry, MV_ITEM_DESC_ROOT | MV_ITEM_DESC_CONTAINER | MV_ITEM_DESC_ALWAYS_DRAW, StorageValueTypes::None, 1);
    class mvColorMapRegistry : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvColorMapRegistry, add_colormap_registry)

        MV_START_COMMANDS
        MV_END_COMMANDS

        MV_START_CONSTANTS
        MV_END_CONSTANTS

    public:

        mvColorMapRegistry(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        bool canChildBeAdded(mvAppItemType type) override;
        void onChildAdd(mvRef<mvAppItem> item) override;
        void alternativeCustomAction() override;

    };

}
