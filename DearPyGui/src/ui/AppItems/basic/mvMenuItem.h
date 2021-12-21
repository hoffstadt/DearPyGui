#pragma once

#include "mvItemRegistry.h"
#include "mvContext.h"

namespace Marvel {

    class mvMenuItem : public mvAppItem
    {

    public:

        explicit mvMenuItem(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

        // values
        void setDataSource(mvUUID dataSource) override;
        void* getValue() override { return &_value; }
        PyObject* getPyValue() override;
        void setPyValue(PyObject* value) override;

    private:

        mvRef<bool> _value = CreateRef<bool>(false);
        bool        _disabled_value = false;
        std::string _shortcut;
        bool        _check = false;

    };

}
