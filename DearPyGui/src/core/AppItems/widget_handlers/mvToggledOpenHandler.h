#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvToggledOpenHandler : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvToggledOpenHandler, add_item_toggled_open_handler)
        MV_NO_COMMANDS
        MV_DEFAULT_CHILDREN

        MV_START_PARENTS
            MV_ADD_PARENT(mvAppItemType::mvStage),
            MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
            MV_ADD_PARENT(mvAppItemType::mvItemHandlerRegistry)
        MV_END_PARENTS

    public:

        explicit mvToggledOpenHandler(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override {}
        void customAction(void* data = nullptr) override;
    };

}
