#pragma once

#include <array>
#include "mvItemRegistry.h"

struct mvColorButtonConfig;
struct mvColorEditConfig;
struct mvColorPickerConfig;
struct mvColorMapConfig;
struct mvColorMapButtonConfig;
struct mvColorMapScaleConfig;
struct mvColorMapSliderConfig;

namespace DearPyGui
{
    // draw commands
    void draw_color_button(ImDrawList* drawlist, mvAppItem& item, mvColorButtonConfig& config);
    void draw_color_edit(ImDrawList* drawlist, mvAppItem& item, mvColorEditConfig& config);
    void draw_color_picker(ImDrawList* drawlist, mvAppItem& item, mvColorPickerConfig& config);
    void draw_color_map(ImDrawList* drawlist, mvAppItem& item, mvColorMapConfig& config);
    void draw_color_map_button(ImDrawList* drawlist, mvAppItem& item, mvColorMapButtonConfig& config);
    void draw_color_map_scale(ImDrawList* drawlist, mvAppItem& item, mvColorMapScaleConfig& config);
    void draw_color_map_slider(ImDrawList* drawlist, mvAppItem& item, mvColorMapSliderConfig& config);
    void draw_color_map_registry(ImDrawList* drawlist, mvAppItem& item);

    // positional args TODO: combine with above
    void set_positional_configuration(PyObject* inDict, mvColorButtonConfig& outConfig);
    void set_positional_configuration(PyObject* inDict, mvColorEditConfig& outConfig);
    void set_positional_configuration(PyObject* inDict, mvColorPickerConfig& outConfig);
    void set_positional_configuration(PyObject* inDict, mvColorMapConfig& outConfig, mvAppItem& item);

    // specific part of `configure_item(...)`
    void set_configuration(PyObject* inDict, mvColorButtonConfig& outConfig);
    void set_configuration(PyObject* inDict, mvColorEditConfig& outConfig);
    void set_configuration(PyObject* inDict, mvColorPickerConfig& outConfig);
    void set_configuration(PyObject* inDict, mvColorMapScaleConfig& outConfig);

    // specific part of `get_item_configuration(...)`
    void fill_configuration_dict(const mvColorButtonConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvColorEditConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvColorPickerConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvColorMapScaleConfig& inConfig, PyObject* outDict);

    // data source handling
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvColorButtonConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvColorEditConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvColorPickerConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvColorMapSliderConfig& outConfig);

    // get python value
    PyObject* set_data_source(mvColorButtonConfig& outConfig);
    PyObject* set_data_source(mvColorEditConfig& outConfig);
    PyObject* set_data_source(mvColorPickerConfig& outConfig);
    PyObject* set_data_source(mvColorMapSliderConfig& outConfig);

    // set python value
    void set_py_value(PyObject* inDict, mvColorButtonConfig& outConfig);
    void set_py_value(PyObject* inDict, mvColorEditConfig& outConfig);
    void set_py_value(PyObject* inDict, mvColorPickerConfig& outConfig);
    void set_py_value(PyObject* inDict, mvColorMapSliderConfig& outConfig);
}

struct mvColorButtonConfig
{

    std::shared_ptr<std::array<float, 4>> value = std::make_shared<std::array<float, 4>>(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
    float                       disabled_value[4]{};
    ImGuiColorEditFlags         flags = ImGuiColorEditFlags_None;
    bool                        no_border = false;

};

struct mvColorEditConfig
{

    std::shared_ptr<std::array<float, 4>> value = std::make_shared<std::array<float, 4>>(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
    float                       disabled_value[4]{};
    ImGuiColorEditFlags         flags = ImGuiColorEditFlags_DefaultOptions_;
    bool                        no_picker = false;
    bool                        no_options = false;
    bool                        no_inputs = false;
    bool                        no_label = false;
    bool                        alpha_bar = false;

};

struct mvColorPickerConfig
{
    std::shared_ptr<std::array<float, 4>> value = std::make_shared<std::array<float, 4>>(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
    float                       disabled_value[4]{};
    ImGuiColorEditFlags         flags = ImGuiColorEditFlags_DefaultOptions_;
    bool                        no_inputs = false;
    bool                        no_label = false;
    bool                        no_side_preview = false;
    bool                        alpha_bar = false;
};

struct mvColorMapConfig
{
    ImPlotColormap      colorMap = -1;
    bool                qualitative = true;
    std::vector<ImVec4> colors;
};

struct mvColorMapButtonConfig
{
    ImPlotColormap  colorMap = 0;
};

struct mvColorMapScaleConfig
{
    double                      scale_min = 0;
    double                      scale_max = 0;
    ImPlotColormap              colorMap = 0;
    std::string                 format = "%g";
    ImPlotColormapScaleFlags    flags = ImPlotColormapScaleFlags_None;
};

struct mvColorMapSliderConfig
{
    std::shared_ptr<float>    value = std::make_shared<float>(0.0f);
    ImVec4          color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    ImPlotColormap  colorMap = 0;
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
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return DearPyGui::set_data_source(configData); }
    void setPyValue(PyObject* value) override { return DearPyGui::set_py_value(value, configData); }
};

class mvColorPicker : public mvAppItem
{
public:
    mvColorPickerConfig configData{};
    explicit mvColorPicker(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_color_picker(drawlist, *this, configData); }
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_positional_configuration(dict, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return DearPyGui::set_data_source(configData); }
    void setPyValue(PyObject* value) override { return DearPyGui::set_py_value(value, configData); }
};

class mvColorMap : public mvAppItem
{
public:
    mvColorMapConfig configData{};
    explicit mvColorMap(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_color_map(drawlist, *this, configData); }
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_positional_configuration(dict, configData, *this); }
};

class mvColorMapButton : public mvAppItem
{
public:
    mvColorMapButtonConfig configData{};
    explicit mvColorMapButton(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_color_map_button(drawlist, *this, configData); }
};

class mvColorMapRegistry : public mvAppItem
{
public:
    explicit mvColorMapRegistry(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_color_map_registry(drawlist, *this); }
};

class mvColorMapScale : public mvAppItem
{
public:
    mvColorMapScaleConfig configData{};
    explicit mvColorMapScale(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_color_map_scale(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
};

class mvColorMapSlider : public mvAppItem
{
public:
    mvColorMapSliderConfig configData{};
    explicit mvColorMapSlider(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_color_map_slider(drawlist, *this, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return DearPyGui::set_data_source(configData); }
    void setPyValue(PyObject* value) override { return DearPyGui::set_py_value(value, configData); }
};