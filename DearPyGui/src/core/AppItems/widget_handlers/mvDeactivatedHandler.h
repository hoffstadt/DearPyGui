#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvDeactivatedHandler : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDeactivatedHandler, add_item_deactivated_handler)
        MV_DEFAULT_CHILDREN

    public:

        explicit mvDeactivatedHandler(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override {}
        void customAction(void* data = nullptr) override;

    };

}
