#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvFloatValue : public mvAppItem
    {

    public:

        explicit mvFloatValue(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override {}

        // values
        void setDataSource(mvUUID dataSource) override;
        void* getValue() override { return &_value; }
        PyObject* getPyValue() override;
        void setPyValue(PyObject* value) override;

    private:

        mvRef<float> _value = CreateRef<float>(0.0f);
        float  _disabled_value = 0.0f;

    };

}
