#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvTooltip : public mvAppItem
    {

    public:

        explicit mvTooltip(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificRequiredArgs(PyObject* dict) override;

    };

}
