#pragma once

#include <array>
#include "mvItemRegistry.h"
#include "mvDearPyGui.h"

namespace DearPyGui 
{
    // draw commands
    void draw_color_button(ImDrawList* drawlist, mvAppItem& item, mvColorButtonConfig& config);

    // positional args TODO: combine with above
    void set_positional_configuration(PyObject* inDict, mvColorButtonConfig& outConfig);

    // specific part of `configure_item(...)`
    void set_configuration(PyObject* inDict, mvColorButtonConfig& outConfig);

    // specific part of `get_item_configuration(...)`
    void fill_configuration_dict(const mvColorButtonConfig& inConfig, PyObject* outDict);

    // template specifics
    void apply_template(const mvColorButtonConfig& sourceConfig, mvColorButtonConfig& dstConfig);

    // data source handling
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvColorButtonConfig& outConfig);

}

struct mvColorButtonConfig
{

    mvRef<std::array<float, 4>> value = CreateRef<std::array<float, 4>>(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
    float                       disabled_value[4]{};
    ImGuiColorEditFlags         flags = ImGuiColorEditFlags_None;
    bool                        no_border = false;

};

class mvColorButton : public mvAppItem
{
public:
    mvColorButtonConfig configData{};
    explicit mvColorButton(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_color_button(drawlist, *this, configData); }
    void handleSpecificPositionalArgs(PyObject * dict) override { DearPyGui::set_positional_configuration(dict, configData); }
    void handleSpecificKeywordArgs(PyObject * dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject * dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem * item) override { auto titem = static_cast<mvColorButton*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override;
    void setPyValue(PyObject * value) override;
};