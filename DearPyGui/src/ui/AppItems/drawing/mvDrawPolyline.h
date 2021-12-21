#pragma once

#include "mvAppItem.h"
#include "mvItemRegistry.h"

namespace Marvel {

    class mvDrawPolyline : public mvAppItem
    {

    public:

        explicit mvDrawPolyline(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificRequiredArgs(PyObject* args) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;


    private:

        std::vector<mvVec4> _points;
        mvColor             _color;
        bool                _closed = false;
        float               _thickness = 1.0f;

    };

}
