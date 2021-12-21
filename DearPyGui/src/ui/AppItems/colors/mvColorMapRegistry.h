#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"
#include "mvModule_DearPyGui.h"
#include "cpp.hint"

namespace Marvel {

    class mvColorMapRegistry : public mvAppItem
    {

    public:

        explicit mvColorMapRegistry(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;

    };

}
