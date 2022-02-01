#pragma once

#include "mvItemRegistry.h"

class mvPlotLegend : public mvAppItem
{

public:

    explicit mvPlotLegend(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void applySpecificTemplate(mvAppItem* item) override;
    void getSpecificConfiguration(PyObject* dict) override;

public:

    ImPlotLocation _legendLocation = ImPlotLocation_NorthWest;
    bool           _horizontal = false;
    bool           _outside = false;
    bool           _dirty = true;
};