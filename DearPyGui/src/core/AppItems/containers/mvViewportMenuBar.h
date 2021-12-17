#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvViewportMenuBar : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvViewportMenuBar, add_viewport_menu_bar)

    public:

        explicit mvViewportMenuBar(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;

    };

}
