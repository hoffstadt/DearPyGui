#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    MV_REGISTER_WIDGET(mvViewportMenuBar);
    class mvViewportMenuBar : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvViewportMenuBar, add_viewport_menu_bar)
        MV_NO_COMMANDS
        MV_DEFAULT_PARENTS
        MV_DEFAULT_CHILDREN
        MV_NO_CONSTANTS

        // TODO: apply appropriately
        MV_SET_STATES(MV_STATE_NONE);

    public:

        explicit mvViewportMenuBar(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;

    };

}
