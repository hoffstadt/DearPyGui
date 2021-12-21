#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvCollapsingHeader : public mvAppItem
    {
    public:

        explicit mvCollapsingHeader(mvUUID uuid);

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
        
        mvRef<bool>        _value = CreateRef<bool>(false);
        bool               _disabled_value = false;
        ImGuiTreeNodeFlags _flags = ImGuiTreeNodeFlags_None;
        bool               _closable = false;

    };

}
