#pragma once

#include <array>
#include "mvItemRegistry.h"

namespace Marvel {

    class mvColorButton : public mvAppItem
    {

    public:

        explicit mvColorButton(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificPositionalArgs(PyObject* dict) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

        // values
        void setDataSource(mvUUID dataSource) override;
        void* getValue() override { return &_value; }
        PyObject* getPyValue() override;
        void setPyValue(PyObject* value) override;

    private:

        mvRef<std::array<float, 4>> _value = CreateRef<std::array<float, 4>>(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
        float                       _disabled_value[4]{};
        ImGuiColorEditFlags         _flags = ImGuiColorEditFlags_None;
        bool                        _no_border = false;

    };

}
