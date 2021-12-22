#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include "dearpygui.h"
#include "cpp.hint"

namespace Marvel {

    class mvItemHandlerRegistry : public mvAppItem
    {

    public:

        explicit mvItemHandlerRegistry(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override {}
        void customAction(void* data = nullptr) override;
        void onBind(mvAppItem* item) override;

    };

}
