#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvHoverHandler : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvHoverHandler, add_item_hover_handler)

    public:

        explicit mvHoverHandler(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override {}
        void customAction(void* data = nullptr) override;

    };

}
