#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include "mvModule_DearPyGui.h"
#include "cpp.hint"

namespace Marvel {

    class mvItemHandlerRegistry : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvItemHandlerRegistry, add_item_handler_registry)

    public:

        explicit mvItemHandlerRegistry(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override {}
        void customAction(void* data = nullptr) override;
        void onBind(mvAppItem* item) override;

    };

}
