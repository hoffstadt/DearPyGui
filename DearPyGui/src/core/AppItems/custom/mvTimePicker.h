#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvTimePicker : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTimePicker, add_time_picker)
        MV_NO_COMMANDS
        MV_DEFAULT_PARENTS
        MV_DEFAULT_CHILDREN

    public:

        explicit mvTimePicker(mvUUID uuid);

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

}
