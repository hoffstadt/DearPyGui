#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvActivatedHandler : public mvAppItem
    {

    public:

        explicit mvActivatedHandler(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override {}
        void customAction(void* data = nullptr) override;

    };

}
