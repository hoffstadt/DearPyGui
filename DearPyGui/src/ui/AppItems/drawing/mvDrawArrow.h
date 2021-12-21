#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"

namespace Marvel {

    class mvDrawArrow : public mvAppItem
    {

    public:

        explicit mvDrawArrow(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificRequiredArgs(PyObject* args) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

    private:

        void updatePoints();

    private:

        mvVec4  _p1 = { 0.0f, 0.0f, 0.0f, 1.0f };
        mvVec4  _p2 = { 0.0f, 0.0f, 0.0f, 1.0f };
        mvColor _color;
        float   _thickness = 1.0f;
        float   _size = 4;
        mvVec4  _points[3];

    };

}
