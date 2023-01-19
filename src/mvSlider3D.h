#pragma once

#include <string>
#include <array>
#include "mvItemRegistry.h"
#include "mvContext.h"
#include "dearpygui.h"

class mvSlider3D : public mvAppItem
{

public:

    explicit mvSlider3D(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;

    // values
    void setDataSource(mvUUID dataSource) override;
    void* getValue() override { return &_value; }
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;

private:

    std::shared_ptr<std::array<float, 4>> _value = std::make_shared<std::array<float, 4>>(std::array<float, 4>{0.0f, 0.0f, 0.0f, 0.0f});
    float                       _disabled_value[4]{};
    float                       _minX = 0.0f;
    float                       _minY = 0.0f;
    float                       _minZ = 0.0f;
    float                       _maxX = 100.0f;
    float                       _maxY = 100.0f;
    float                       _maxZ = 100.0f;
    float                       _scale = 1.0f;

};