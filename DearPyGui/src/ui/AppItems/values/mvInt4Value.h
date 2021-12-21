#pragma once

#include <array>
#include "mvItemRegistry.h"

namespace Marvel {

    class mvInt4Value : public mvAppItem
    {

    public:

        explicit mvInt4Value(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override {}

        // values
        void setDataSource(mvUUID dataSource) override;
        void* getValue() override { return &_value; }
        PyObject* getPyValue() override;
        void setPyValue(PyObject* value) override;

    private:

        mvRef<std::array<int, 4>> _value = CreateRef<std::array<int, 4>>(std::array<int, 4>{0, 0, 0, 0});
        int  _disabled_value[4]{};
    };

}
