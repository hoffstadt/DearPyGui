#pragma once

#include "mvItemRegistry.h"

class mvLoadingIndicator : public mvAppItem
{

public:

    explicit mvLoadingIndicator(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;

    enum Style {
        Style_OldDottedCircle = 0,
        Style_OldRing,
        Style_DottedCircle,
        Style_Ring,
    };

private:

    Style   _style = Style_DottedCircle;
    int     _circleCount = 8;
    float   _radius = 3.0f;
    float   _speed = 1.0f;
    float   _thickness = 1.0f;
    mvColor _mainColor;
    mvColor _optionalColor;

};