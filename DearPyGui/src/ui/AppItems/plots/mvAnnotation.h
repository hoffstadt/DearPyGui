#pragma once

#include <array>
#include "mvItemRegistry.h"

class mvAnnotation : public mvAppItem
{

public:

    explicit mvAnnotation(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;
    void applySpecificTemplate(mvAppItem* item) override;
        
    // values
    void setDataSource(mvUUID dataSource) override;
    void* getValue() override { return &_value; }
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;

private:

    mvRef<std::array<double, 4>> _value = CreateRef<std::array<double, 4>>(std::array<double, 4>{0.0, 0.0, 0.0, 0.0});
    double                       _disabled_value[4]{};
    mvColor                      _color = mvColor(0.0f, 0.0f, 0.0f, -1.0f);
    bool                         _clamped = true;
    ImVec2                       _pixOffset;

};