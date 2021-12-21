#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"

namespace Marvel {

    class mvDrawEllipse : public mvAppItem
    {

    public:

        explicit mvDrawEllipse(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificRequiredArgs(PyObject* args) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;


    private:

        mvVec4  _pmin = { 0.0f, 0.0f, 0.0f, 1.0f };
        mvVec4  _pmax = { 1.0f, 1.0f, 0.0f, 1.0f };
        int     _segments = 0;
        mvColor _color;
        mvColor _fill;
        float   _thickness = 1.0f;
        std::vector<mvVec4> _points;
        bool _dirty = true;

    };

}
