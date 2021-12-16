#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvFocusHandler : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvFocusHandler, add_item_focus_handler)
        MV_DEFAULT_CHILDREN

    public:

        explicit mvFocusHandler(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override {}
        void customAction(void* data = nullptr) override;

    };

}
