#pragma once

#include <array>
#include "mvItemRegistry.h"

class mvColorPicker : public mvAppItem
{

public:

    explicit mvColorPicker(mvUUID uuid) : mvAppItem(uuid) {}

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
    ImGuiColorEditFlags         _flags = ImGuiColorEditFlags__OptionsDefault;
    bool                        _no_inputs = false;
    bool                        _no_label = false;
    bool                        _no_side_preview = false;
    bool                        _alpha_bar = false;

};