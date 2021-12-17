#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvDeactivatedAfterEditHandler : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDeactivatedAfterEditHandler, add_item_deactivated_after_edit_handler)

    public:

        explicit mvDeactivatedAfterEditHandler(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override {}
        void customAction(void* data = nullptr) override;

    };

}
