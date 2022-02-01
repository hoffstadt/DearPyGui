#pragma once

#include "mvItemRegistry.h"
#include "mvContext.h"
#include "dearpygui.h"
#include <string>

class mvColorMap : public mvAppItem
{

public:

    explicit mvColorMap(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificRequiredArgs(PyObject* args) override;
    void applySpecificTemplate(mvAppItem* item) override;
    ImPlotColormap getColorMap() const { return _colorMap; }

public:

    ImPlotColormap      _colorMap = -1;
    bool                _qualitative = true;
    std::vector<ImVec4> _colors;
};