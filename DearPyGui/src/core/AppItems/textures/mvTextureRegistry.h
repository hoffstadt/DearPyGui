#pragma once
#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include "mvModule_Core.h"
#include "cpp.hint"

namespace Marvel {

    MV_REGISTER_WIDGET(mvTextureRegistry, MV_ITEM_DESC_ROOT | MV_ITEM_DESC_CONTAINER | MV_ITEM_DESC_ALWAYS_DRAW,
        StorageValueTypes::None, 1);
    class mvTextureRegistry : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTextureRegistry, add_texture_registry)

        MV_START_COMMANDS
        MV_END_COMMANDS

        MV_START_CONSTANTS
        MV_END_CONSTANTS

    public:

        mvTextureRegistry(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;

        bool canChildBeAdded(mvAppItemType type) override;
        void onChildRemoved(mvRef<mvAppItem> item) override;
        void onChildrenRemoved() override;

    private:

        void show_debugger();

    private:

        int m_selection = -1;
    };

}
