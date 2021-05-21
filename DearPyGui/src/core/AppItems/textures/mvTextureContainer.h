#pragma once
#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include "mvModule_Core.h"
#include "cpp.hint"

namespace Marvel {

    MV_REGISTER_WIDGET(mvTextureContainer, MV_ITEM_DESC_ROOT | MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 1);
    class mvTextureContainer : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTextureContainer, add_texture_container)

        MV_START_EXTRA_COMMANDS
        MV_END_EXTRA_COMMANDS

        MV_START_GENERAL_CONSTANTS
        MV_END_GENERAL_CONSTANTS

        MV_START_COLOR_CONSTANTS
        MV_END_COLOR_CONSTANTS

        MV_START_STYLE_CONSTANTS
        MV_END_STYLE_CONSTANTS

    public:

        mvTextureContainer(const std::string& name);

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
