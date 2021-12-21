#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvActiveHandler : public mvAppItem
    {

    public:

        explicit mvActiveHandler(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override {}
        void customAction(void* data = nullptr) override;

    };

}
