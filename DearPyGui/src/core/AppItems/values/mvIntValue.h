#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvIntValue : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvIntValue, add_int_value)
        MV_DEFAULT_CHILDREN

    public:

        explicit mvIntValue(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override {}

        // values
        void setDataSource(mvUUID dataSource) override;
        void* getValue() override { return &_value; }
        PyObject* getPyValue() override;
        void setPyValue(PyObject* value) override;

    private:

        mvRef<int> _value = CreateRef<int>(0);
        int        _disabled_value = 0;

    };

}
