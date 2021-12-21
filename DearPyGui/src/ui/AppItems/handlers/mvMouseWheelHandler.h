#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvMouseWheelHandler : public mvAppItem
    {

    public:

        explicit mvMouseWheelHandler(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;

    };

}
