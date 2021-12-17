#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"

namespace Marvel {

    class mvMenuBar : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvMenuBar, add_menu_bar)

    public:

        explicit mvMenuBar(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;

    };

}
