#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvDatePicker : public mvAppItem
    {
        enum class DatePickerLevel {
            mvDatePickerLevel_Day = 0L,
            mvDatePickerLevel_Month,
            mvDatePickerLevel_Year
        };

    public:

        explicit mvDatePicker(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
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
        int               _level = 0;

    };

}
