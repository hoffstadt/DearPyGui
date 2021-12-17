#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvToggledOpenHandler : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvToggledOpenHandler, add_item_toggled_open_handler)

    public:

        explicit mvToggledOpenHandler(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override {}
        void customAction(void* data = nullptr) override;
    };

}
