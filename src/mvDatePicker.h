#pragma once

#include "mvItemRegistry.h"

class mvDatePicker : public mvAppItem
{
    enum class DatePickerLevel {
        mvDatePickerLevel_Day = 0L,
        mvDatePickerLevel_Month,
        mvDatePickerLevel_Year
    };

public:

    explicit mvDatePicker(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;

    // values
    void* getValue() override { return &_value; }
    PyObject* getPyValue() override;
    void setPyValue(PyObject* value) override;

private:

    std::shared_ptr<tm>         _value = std::make_shared<tm>();
    std::shared_ptr<ImPlotTime> _imvalue = std::make_shared<ImPlotTime>();
    int               _level = 0;

};