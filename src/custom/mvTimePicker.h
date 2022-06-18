#pragma once

#include "mvItemRegistry.h"

class mvTimePicker : public mvAppItem
{

public:

    explicit mvTimePicker(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y)override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;
    void applySpecificTemplate(mvAppItem* item) override;

    // values
    void* getValue() override { return &_value; }
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;

private:
    mvRef<tm>         _value = CreateRef<tm>();
    mvRef<ImPlotTime> _imvalue = CreateRef<ImPlotTime>();
    bool              _hour24 = false;

};