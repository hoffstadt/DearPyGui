#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"

namespace Marvel {

    class mvDrawLayer : public mvAppItem
    {

    public:

        explicit mvDrawLayer(mvUUID uuid);
        ~mvDrawLayer();

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;

    };

}
