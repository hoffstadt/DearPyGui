#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvEditedHandler : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvEditedHandler, add_item_edited_handler)
        MV_DEFAULT_CHILDREN

    public:

        explicit mvEditedHandler(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override {}
        void customAction(void* data = nullptr) override;

    };

}
