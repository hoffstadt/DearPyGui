#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"

namespace Marvel {

    class mvDrawCircle : public mvAppItem
    {

    public:

        explicit mvDrawCircle(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificRequiredArgs(PyObject* args) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;


    private:

        mvVec4  _center = { 0.0f, 0.0f, 0.0f, 1.0f };
        float   _radius = 1.0f;
        int     _segments = 0;
        mvColor _color;
        mvColor _fill;
        float   _thickness = 1.0f;

    };

}
