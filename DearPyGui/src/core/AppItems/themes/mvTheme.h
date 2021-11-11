#pragma once

#include "mvItemRegistry.h"
#include "mvThemeComponent.h"

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
            MV_ADD_CHILD(mvAppItemType::mvThemeComponent)
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
        void setSpecificType(int specificType) { _specificType = specificType; }
        void setSpecificEnabled(int enabled) { _specificEnabled = enabled; }

    public:

        int _specificType = (int)mvAppItemType::All;
        bool _specificEnabled = true;

    };
}
