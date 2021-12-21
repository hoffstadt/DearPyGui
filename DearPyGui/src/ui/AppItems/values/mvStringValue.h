#pragma once

#include <string>
#include "mvItemRegistry.h"

namespace Marvel {

    class mvStringValue : public mvAppItem
    {

    public:

        explicit mvStringValue(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override {}

        // values
        void setDataSource(mvUUID dataSource) override;
        void* getValue() override { return &_value; }
        PyObject* getPyValue() override;
        void setPyValue(PyObject* value) override;

    private:

        mvRef<std::string> _value = CreateRef<std::string>("");
        std::string  _disabled_value = "";
    };

}
