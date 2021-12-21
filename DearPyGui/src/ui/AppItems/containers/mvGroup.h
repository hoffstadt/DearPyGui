#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvGroup : public mvAppItem
    {

    public:

        explicit mvGroup(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

    private:

        bool  _horizontal = false;
        float _hspacing = -1.0f;
        float _xoffset = 0.0f;

    };

}
