#pragma once

#include "mvItemRegistry.h"

class mvTooltip : public mvAppItem
{

public:

    explicit mvTooltip(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificRequiredArgs(PyObject* dict) override;

};