#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvVisibleHandler : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvVisibleHandler, add_item_visible_handler)

    public:

        explicit mvVisibleHandler(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override {}
        void customAction(void* data = nullptr) override;

    };

}
