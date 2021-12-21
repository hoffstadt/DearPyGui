#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvSpacer : public mvAppItem
    {

    public:

        explicit mvSpacer(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;

    };

}
