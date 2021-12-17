#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvResizeHandler : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvResizeHandler, add_item_resize_handler)

    public:

        explicit mvResizeHandler(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override {}
        void customAction(void* data = nullptr) override;

    };

}
