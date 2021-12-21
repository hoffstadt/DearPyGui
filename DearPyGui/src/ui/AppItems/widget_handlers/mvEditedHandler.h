#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvEditedHandler : public mvAppItem
    {

    public:

        explicit mvEditedHandler(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override {}
        void customAction(void* data = nullptr) override;

    };

}
