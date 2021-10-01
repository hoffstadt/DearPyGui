#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    MV_REGISTER_WIDGET(mvDatePicker, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Time, 1);
    class mvDatePicker : public mvAppItem
    {
        enum class DatePickerLevel {
            mvDatePickerLevel_Day = 0L,
            mvDatePickerLevel_Month,
            mvDatePickerLevel_Year
        };

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvDatePicker, add_date_picker)
        MV_NO_COMMANDS
        MV_DEFAULT_PARENTS
        MV_DEFAULT_CHILDREN

        MV_SET_STATES(
            MV_STATE_HOVER |
            MV_STATE_ACTIVE |
            MV_STATE_FOCUSED |
            MV_STATE_CLICKED |
            MV_STATE_VISIBLE |
            MV_STATE_ACTIVATED |
            MV_STATE_DEACTIVATED |
            MV_STATE_RECT_MIN |
            MV_STATE_RECT_MAX |
            MV_STATE_RECT_SIZE |
            MV_STATE_CONT_AVAIL
        );

        MV_CREATE_CONSTANT(mvDatePickerLevel_Day, 0L);
        MV_CREATE_CONSTANT(mvDatePickerLevel_Month, 1L);
        MV_CREATE_CONSTANT(mvDatePickerLevel_Year, 2L);

        MV_START_CONSTANTS
            MV_ADD_CONSTANT(mvDatePickerLevel_Day),
            MV_ADD_CONSTANT(mvDatePickerLevel_Month),
            MV_ADD_CONSTANT(mvDatePickerLevel_Year)
        MV_END_CONSTANTS

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
