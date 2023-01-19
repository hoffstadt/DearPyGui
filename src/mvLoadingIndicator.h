#pragma once

#include "mvItemRegistry.h"

class mvLoadingIndicator : public mvAppItem
{

public:

    explicit mvLoadingIndicator(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;

private:

    int     _style = 0;
    int     _circleCount = 8;
    float   _radius = 3.0f;
    float   _speed = 1.0f;
    float   _thickness = 1.0f;
    mvColor _mainColor = mvColor(51, 51, 55, 255);
    mvColor _optionalColor = mvColor(29, 151, 236, 103);

};