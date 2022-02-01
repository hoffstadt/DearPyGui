#pragma once

#include <array>
#include "mvItemRegistry.h"

class mvColorMapSlider : public mvAppItem
{

public:

    explicit mvColorMapSlider(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override;
    void applySpecificTemplate(mvAppItem* item) override;
    void setColorMap(ImPlotColormap colormap) { _colormap = colormap; }

    // values
    void setDataSource(mvUUID dataSource) override;
    void* getValue() override { return &_value; }
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;

private:

    mvRef<float>    _value = CreateRef<float>(0.0f);
    ImVec4          _color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    ImPlotColormap  _colormap = 0;

};