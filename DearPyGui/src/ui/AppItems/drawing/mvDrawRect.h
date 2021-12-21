#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"

namespace Marvel {

    class mvDrawRect : public mvAppItem
    {

    public:

        explicit mvDrawRect(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificRequiredArgs(PyObject* args) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

    private:

        mvVec4  _pmax = { 0.0f, 0.0f, 0.0f, 1.0f };
        mvVec4  _pmin = { 1.0f, 0.0f, 0.0f, 1.0f };
        float   _rounding = 0.0f;
        mvColor _color;
        mvColor _color_upper_left;
        mvColor _color_upper_right;
        mvColor _color_bottom_left;
        mvColor _color_bottom_right;
        mvColor _fill;
        float   _thickness = 1.0f;
        bool    _multicolor = false;
    };

}
