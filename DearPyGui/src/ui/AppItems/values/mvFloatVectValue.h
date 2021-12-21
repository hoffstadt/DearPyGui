#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvFloatVectValue : public mvAppItem
    {

    public:

        explicit mvFloatVectValue(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override {}

        // values
        void setDataSource(mvUUID dataSource) override;
        void* getValue() override { return &_value; }
        PyObject* getPyValue() override;
        void setPyValue(PyObject* value) override;

    private:

        mvRef<std::vector<float>> _value = CreateRef<std::vector<float>>(std::vector<float>{0.0f});

    };

}
