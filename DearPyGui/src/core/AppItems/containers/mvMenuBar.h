#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"

namespace Marvel {

    MV_REGISTER_WIDGET(mvMenuBar, MV_ITEM_DESC_CONTAINER, StorageValueTypes::Bool, 1);
    class mvMenuBar : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvMenuBar, add_menu_bar)
        MV_NO_COMMANDS
        MV_DEFAULT_CHILDREN
        MV_NO_CONSTANTS

        MV_SET_STATES(MV_STATE_VISIBLE);

        MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvWindowAppItem),
            MV_ADD_PARENT(mvAppItemType::mvChildWindow),
            MV_ADD_PARENT(mvAppItemType::mvNodeEditor),
            MV_ADD_PARENT(mvAppItemType::mvStage),
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
        MV_END_PARENTS

    public:

        explicit mvMenuBar(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;

    };

}
