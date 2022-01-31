#pragma once

#include <array>
#include "mvItemRegistry.h"

class mvFloat4Value : public mvAppItem
{

public:

    explicit mvFloat4Value(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override {}

    // values
    void setDataSource(mvUUID dataSource) override;
    void* getValue() override { return &_value; }
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;

private:

    mvRef<std::array<float, 4>> _value = CreateRef<std::array<float, 4>>(std::array<float, 4>{0.0f, 0.0f, 0.0f, 0.0f});
    float  _disabled_value[4]{};

};