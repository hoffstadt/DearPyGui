#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvIntValue : public mvAppItem
    {

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
