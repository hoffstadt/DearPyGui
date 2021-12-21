#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvBoolValue : public mvAppItem
    {

    public:

        explicit mvBoolValue(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override {}

        // values
        void setDataSource(mvUUID dataSource) override;
        void* getValue() override { return &_value; }
        PyObject* getPyValue() override;
        void setPyValue(PyObject* value) override;

    private:

        mvRef<bool> _value = CreateRef<bool>(false);
        bool  _disabled_value = false;
    };

}
