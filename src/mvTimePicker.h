#pragma once

#include "mvItemRegistry.h"

class mvTimePicker : public mvAppItem
{

public:

    explicit mvTimePicker(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y)override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;

    // values
    void* getValue() override { return &_value; }
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;

private:
    std::shared_ptr<tm>         _value = std::make_shared<tm>();
    std::shared_ptr<ImPlotTime> _imvalue = std::make_shared<ImPlotTime>();
    bool              _hour24 = false;

};