#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvActivatedHandler : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvActivatedHandler, add_item_activated_handler)
        MV_DEFAULT_CHILDREN

    public:

        explicit mvActivatedHandler(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override {}
        void customAction(void* data = nullptr) override;

    };

}
