#pragma once

#include <array>
#include "mvItemRegistry.h"
#include "mvDearPyGui.h"

namespace DearPyGui 
{
    // draw commands
    void draw_color_button(ImDrawList* drawlist, mvAppItem& item, mvColorButtonConfig& config);
    void draw_color_edit(ImDrawList* drawlist, mvAppItem& item, mvColorEditConfig& config);

    // positional args TODO: combine with above
    void set_positional_configuration(PyObject* inDict, mvColorButtonConfig& outConfig);
    void set_positional_configuration(PyObject* inDict, mvColorEditConfig& outConfig);

    // specific part of `configure_item(...)`
    void set_configuration(PyObject* inDict, mvColorButtonConfig& outConfig);
    void set_configuration(PyObject* inDict, mvColorEditConfig& outConfig);

    // specific part of `get_item_configuration(...)`
    void fill_configuration_dict(const mvColorButtonConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvColorEditConfig& inConfig, PyObject* outDict);

    // template specifics
    void apply_template(const mvColorButtonConfig& sourceConfig, mvColorButtonConfig& dstConfig);
    void apply_template(const mvColorEditConfig& sourceConfig, mvColorEditConfig& dstConfig);

    // data source handling
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvColorButtonConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvColorEditConfig& outConfig);

    // get python value
    PyObject* set_data_source(mvColorButtonConfig& outConfig);
    PyObject* set_data_source(mvColorEditConfig& outConfig);

    // set python value
    void set_py_value(PyObject* inDict, mvColorButtonConfig& outConfig);
    void set_py_value(PyObject* inDict, mvColorEditConfig& outConfig);
}

struct mvColorButtonConfig
{

    mvRef<std::array<float, 4>> value = CreateRef<std::array<float, 4>>(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
    float                       disabled_value[4]{};
    ImGuiColorEditFlags         flags = ImGuiColorEditFlags_None;
    bool                        no_border = false;

};

struct mvColorEditConfig
{

    mvRef<std::array<float, 4>> value = CreateRef<std::array<float, 4>>(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
    float                       disabled_value[4]{};
    ImGuiColorEditFlags         flags = ImGuiColorEditFlags__OptionsDefault;
    bool                        no_picker = false;
    bool                        no_options = false;
    bool                        no_inputs = false;
    bool                        no_label = false;
    bool                        alpha_bar = false;

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
    PyObject* getPyValue() override { return DearPyGui::set_data_source(configData); }
    void setPyValue(PyObject* value) override { return DearPyGui::set_py_value(value, configData); }
};

class mvColorEdit : public mvAppItem
{
public:
    mvColorEditConfig configData{};
    explicit mvColorEdit(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_color_edit(drawlist, *this, configData); }
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_positional_configuration(dict, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvColorEdit*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return DearPyGui::set_data_source(configData); }
    void setPyValue(PyObject* value) override { return DearPyGui::set_py_value(value, configData); }
};