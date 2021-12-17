#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvActiveHandler : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvActiveHandler, add_item_active_handler)

    public:

        explicit mvActiveHandler(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override {}
        void customAction(void* data = nullptr) override;

    };

}
