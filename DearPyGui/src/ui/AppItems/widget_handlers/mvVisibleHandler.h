#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvVisibleHandler : public mvAppItem
    {

    public:

        explicit mvVisibleHandler(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override {}
        void customAction(void* data = nullptr) override;

    };

}
