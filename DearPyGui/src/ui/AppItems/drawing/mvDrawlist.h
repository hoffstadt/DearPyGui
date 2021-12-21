#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvDrawlist : public mvAppItem
    {

    public:

        explicit mvDrawlist(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificRequiredArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

    private:

        float  _startx = 0.0f;
        float  _starty = 0.0f;

    };
}
