#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"

namespace Marvel {

    class mvDrawPolygon : public mvAppItem
    {

    public:

        explicit mvDrawPolygon(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificRequiredArgs(PyObject* args) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;


    private:

        std::vector<mvVec4> _points;
        mvColor             _color;
        mvColor             _fill;
        float               _thickness = 1.0f;

    };

}
