#pragma once

#include "mvItemRegistry.h"
#include "mvDearPyGui.h"
#include <array>

namespace DearPyGui
{
    // specific part of `get_item_configuration(...)`
    void fill_configuration_dict(const mvButtonConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvComboConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvDragFloatConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvDragFloatMultiConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvDragIntConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvDragIntMultiConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvSliderIntConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvSliderIntMultiConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvSliderFloatConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvSliderFloatMultiConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvListboxConfig& inConfig, PyObject* outDict);

    // specific part of `configure_item(...)`
    void set_configuration(PyObject* inDict, mvButtonConfig& outConfig);
    void set_configuration(PyObject* inDict, mvComboConfig& outConfig);
    void set_configuration(PyObject* inDict, mvDragFloatConfig& outConfig, mvAppItemInfo& info);
    void set_configuration(PyObject* inDict, mvDragIntConfig& outConfig, mvAppItemInfo& info);
    void set_configuration(PyObject* inDict, mvDragIntMultiConfig& outConfig, mvAppItemInfo& info);
    void set_configuration(PyObject* inDict, mvDragFloatMultiConfig& outConfig, mvAppItemInfo& info);
    void set_configuration(PyObject* inDict, mvSliderIntConfig& outConfig, mvAppItemInfo& info);
    void set_configuration(PyObject* inDict, mvSliderIntMultiConfig& outConfig, mvAppItemInfo& info);
    void set_configuration(PyObject* inDict, mvSliderFloatConfig& outConfig, mvAppItemInfo& info);
    void set_configuration(PyObject* inDict, mvSliderFloatMultiConfig& outConfig, mvAppItemInfo& info);
    void set_configuration(PyObject* inDict, mvListboxConfig& outConfig, mvAppItemInfo& info);

    // positional args TODO: combine with above
    void set_positional_configuration(PyObject* inDict, mvComboConfig& outConfig);
    void set_positional_configuration(PyObject* inDict, mvListboxConfig& outConfig);

    // data source handling
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvComboConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvCheckboxConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvDragFloatConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvDragFloatMultiConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvDragIntConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvDragIntMultiConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvSliderFloatConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvSliderFloatMultiConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvSliderIntConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvSliderIntMultiConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvListboxConfig& outConfig);

    // template specifics
    void apply_template(const mvButtonConfig& sourceConfig, mvButtonConfig& dstConfig);
    void apply_template(const mvComboConfig& sourceConfig, mvComboConfig& dstConfig);
    void apply_template(const mvCheckboxConfig& sourceConfig, mvCheckboxConfig& dstConfig);
    void apply_template(const mvDragFloatConfig& sourceConfig, mvDragFloatConfig& dstConfig);
    void apply_template(const mvDragFloatMultiConfig& sourceConfig, mvDragFloatMultiConfig& dstConfig);
    void apply_template(const mvDragIntConfig& sourceConfig, mvDragIntConfig& dstConfig);
    void apply_template(const mvDragIntMultiConfig& sourceConfig, mvDragIntMultiConfig& dstConfig);
    void apply_template(const mvSliderFloatConfig& sourceConfig, mvSliderFloatConfig& dstConfig);
    void apply_template(const mvSliderFloatMultiConfig& sourceConfig, mvSliderFloatMultiConfig& dstConfig);
    void apply_template(const mvSliderIntConfig& sourceConfig, mvSliderIntConfig& dstConfig);
    void apply_template(const mvSliderIntMultiConfig& sourceConfig, mvSliderIntMultiConfig& dstConfig);
    void apply_template(const mvListboxConfig& sourceConfig, mvListboxConfig& dstConfig);


    // draw commands
    void draw_button       (ImDrawList* drawlist, mvAppItem& item, const mvButtonConfig& config);
    void draw_combo        (ImDrawList* drawlist, mvAppItem& item, mvComboConfig& config);
    void draw_checkbox     (ImDrawList* drawlist, mvAppItem& item, mvCheckboxConfig& config);
    void draw_drag_float   (ImDrawList* drawlist, mvAppItem& item, mvDragFloatConfig& config);
    void draw_drag_floatx  (ImDrawList* drawlist, mvAppItem& item, mvDragFloatMultiConfig& config);
    void draw_drag_int     (ImDrawList* drawlist, mvAppItem& item, mvDragIntConfig& config);
    void draw_drag_intx    (ImDrawList* drawlist, mvAppItem& item, mvDragIntMultiConfig& config);
    void draw_slider_float (ImDrawList* drawlist, mvAppItem& item, mvSliderFloatConfig& config);
    void draw_slider_floatx(ImDrawList* drawlist, mvAppItem& item, mvSliderFloatMultiConfig& config);
    void draw_slider_int   (ImDrawList* drawlist, mvAppItem& item, mvSliderIntConfig& config);
    void draw_slider_intx  (ImDrawList* drawlist, mvAppItem& item, mvSliderIntMultiConfig& config);
    void draw_listbox      (ImDrawList* drawlist, mvAppItem& item, mvListboxConfig& config);
    void draw_separator    (ImDrawList* drawlist, mvAppItem& item);
    void draw_spacer       (ImDrawList* drawlist, mvAppItem& item);
    void draw_menubar      (ImDrawList* drawlist, mvAppItem& item);
    void draw_clipper      (ImDrawList* drawlist, mvAppItem& item);
}

enum class mvComboHeightMode
{
    mvComboHeight_Small = 0L,
    mvComboHeight_Regular,
    mvComboHeight_Large,
    mvComboHeight_Largest
};

struct mvButtonConfig
{
    ImGuiDir direction = ImGuiDir_Up;
    bool     small_button = false;
    bool     arrow = false;
};

struct mvComboConfig
{
    ImGuiComboFlags          flags = ImGuiComboFlags_None;
    std::vector<std::string> items;
    bool                     popup_align_left = false;
    bool                     no_preview = false;
    mvRef<std::string>       value = CreateRef<std::string>("");
    std::string              disabled_value;
};

struct mvCheckboxConfig
{
    mvRef<bool> value = CreateRef<bool>(false);
    bool        disabled_value = false;
};

struct mvDragFloatConfig
{
    float               speed = 1.0f;
    float               minv = 0.0f;
    float               maxv = 100.0f;
    std::string         format = "%.3f";
    ImGuiInputTextFlags flags = ImGuiSliderFlags_None;
    ImGuiInputTextFlags stor_flags = ImGuiSliderFlags_None;
    mvRef<float>        value = CreateRef<float>(0.0f);
    float               disabled_value = 0.0f;
};

struct mvDragIntConfig
{
    float               speed = 1.0f;
    int                 minv = 0;
    int                 maxv = 100;
    std::string         format = "%d";
    ImGuiInputTextFlags flags = ImGuiSliderFlags_None;
    ImGuiInputTextFlags stor_flags = ImGuiSliderFlags_None;
    mvRef<int>          value = CreateRef<int>(0);
    int                 disabled_value = 0;
};

struct mvDragIntMultiConfig
{
    float                     speed = 1.0f;
    int                       minv = 0;
    int                       maxv = 100;
    std::string               format = "%d";
    ImGuiInputTextFlags       flags = ImGuiSliderFlags_None;
    ImGuiInputTextFlags       stor_flags = ImGuiSliderFlags_None;
    int                       size = 4;
    mvRef<std::array<int, 4>> value = CreateRef<std::array<int, 4>>(std::array<int, 4>{0, 0, 0, 0});
    int                       disabled_value[4]{}; 
};

struct mvDragFloatMultiConfig
{
    float                       speed = 1.0f;
    float                       minv = 0.0f;
    float                       maxv = 100.0f;
    std::string                 format = "%.3f";
    ImGuiInputTextFlags         flags = ImGuiSliderFlags_None;
    ImGuiInputTextFlags         stor_flags = ImGuiSliderFlags_None;
    int                         size = 4;
    mvRef<std::array<float, 4>> value = CreateRef<std::array<float, 4>>(std::array<float, 4>{0.0f, 0.0f, 0.0f, 0.0f});
    float                       disabled_value[4]{};  
};

struct mvSliderIntConfig
{
    int                 minv = 0;
    int                 maxv = 100;
    std::string         format = "%d";
    bool                vertical = false;
    ImGuiInputTextFlags flags = ImGuiSliderFlags_None;
    ImGuiInputTextFlags stor_flags = ImGuiSliderFlags_None;
    mvRef<int>          value = CreateRef<int>(0);
    int                 disabled_value = 0;
};

struct mvSliderFloatConfig
{
    float               minv = 0.0f;
    float               maxv = 100.0f;
    std::string         format = "%.3f";
    bool                vertical = false;
    ImGuiInputTextFlags flags = ImGuiSliderFlags_None;
    ImGuiInputTextFlags stor_flags = ImGuiSliderFlags_None;
    mvRef<float>        value = CreateRef<float>(0.0f);
    float               disabled_value = 0.0f;
};

struct mvSliderFloatMultiConfig
{
    float                       minv = 0.0f;
    float                       maxv = 100.0f;
    std::string                 format = "%.3f";
    ImGuiInputTextFlags         flags = ImGuiSliderFlags_None;
    ImGuiInputTextFlags         stor_flags = ImGuiSliderFlags_None;
    int                         size = 4;
    mvRef<std::array<float, 4>> value = CreateRef<std::array<float, 4>>(std::array<float, 4>{0.0f, 0.0f, 0.0f, 0.0f});
    float                       disabled_value[4]{};
};

struct mvSliderIntMultiConfig
{
    int                       minv = 0;
    int                       maxv = 100;
    std::string               format = "%d";
    ImGuiInputTextFlags       flags = ImGuiSliderFlags_None;
    ImGuiInputTextFlags       stor_flags = ImGuiSliderFlags_None;
    int                       size = 4;
    mvRef<std::array<int, 4>> value = CreateRef<std::array<int, 4>>(std::array<int, 4>{0, 0, 0, 0});
    int                       disabled_value[4]{};
};

struct mvListboxConfig
{
    std::vector<std::string> names;
    int                      itemsHeight = 3; // number of items to show (default -1)
    std::vector<const char*> charNames;
    int                      index = 0;
    int                      disabledindex = 0;
    mvRef<std::string>       value = CreateRef<std::string>("");
    std::string              disabled_value;
};

//-----------------------------------------------------------------------------
// Old Classes, in the process of removing OOP crap
//-----------------------------------------------------------------------------
class mvButton : public mvAppItem
{
public:
    mvButtonConfig configData{};
    explicit mvButton(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_button(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvButton*>(item); DearPyGui::apply_template(titem->configData, configData); }
};

class mvCombo : public mvAppItem
{
public:
    mvComboConfig configData{};
    explicit mvCombo(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_combo(drawlist, *this, configData); }
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_positional_configuration(dict, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvCombo*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyString(*configData.value); };
    void setPyValue(PyObject* value) override { *configData.value = ToString(value); }
};

class mvCheckbox : public mvAppItem
{
public:
    mvCheckboxConfig configData{};
    explicit mvCheckbox(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_checkbox(drawlist, *this, configData); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvCheckbox*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void setPyValue(PyObject* value) override { *configData.value = ToBool(value); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override{ return ToPyBool(*configData.value); }
};

class mvDragFloat : public mvAppItem
{
public:
    mvDragFloatConfig configData{};
    explicit mvDragFloat(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_drag_float(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData, info); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override{ auto titem = static_cast<mvDragFloat*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override{ return ToPyFloat(*configData.value); }
    void setPyValue(PyObject* value) override{ *configData.value = ToFloat(value); }
};

class mvDragInt : public mvAppItem
{

public:
    mvDragIntConfig configData{};
    explicit mvDragInt(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_drag_int(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData, info); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvDragInt*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyInt(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToInt(value); }

};

class mvDragIntMulti : public mvAppItem
{

public:
    mvDragIntMultiConfig configData{};
    explicit mvDragIntMulti(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_drag_intx(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData, info); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvDragIntMulti*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyIntList(configData.value->data(), 4); }
    void setPyValue(PyObject* value) override;
};

class mvDragFloatMulti : public mvAppItem
{
public:
    mvDragFloatMultiConfig configData{};
    explicit mvDragFloatMulti(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_drag_floatx(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData, info); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvDragFloatMulti*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyFloatList(configData.value->data(), 4); }
    void setPyValue(PyObject* value) override;
};

class mvSliderFloat : public mvAppItem
{
public:
    mvSliderFloatConfig configData{};
    explicit mvSliderFloat(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_slider_float(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData, info); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvSliderFloat*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyFloat(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToFloat(value); }
};

class mvSliderFloatMulti : public mvAppItem
{
public:
    mvSliderFloatMultiConfig configData{};
    explicit mvSliderFloatMulti(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_slider_floatx(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData, info); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvSliderFloatMulti*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyFloatList(configData.value->data(), 4); }
    void setPyValue(PyObject* value) override;
};

class mvSliderInt : public mvAppItem
{
public:
    mvSliderIntConfig configData{};
    explicit mvSliderInt(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_slider_int(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData, info); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvSliderInt*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyInt(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToInt(value); }
};

class mvSliderIntMulti : public mvAppItem
{
public:
    mvSliderIntMultiConfig configData{};
    explicit mvSliderIntMulti(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_slider_intx(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData, info); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvSliderIntMulti*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyIntList(configData.value->data(), 4); }
    void setPyValue(PyObject* value) override;
};

class mvListbox : public mvAppItem
{
public:
    mvListboxConfig configData{};
    explicit mvListbox(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_listbox(drawlist, *this, configData); }
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_positional_configuration(dict, configData);}
    void handleSpecificKeywordArgs(PyObject* dict) override{ DearPyGui::set_configuration(dict, configData, info); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvListbox*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setPyValue(PyObject* value) override;
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override {return ToPyString(*configData.value);}
};

class mvSeparator : public mvAppItem
{
public:
    explicit mvSeparator(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_separator(drawlist, *this); }
};

class mvSpacer : public mvAppItem
{
public:
    explicit mvSpacer(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_spacer(drawlist, *this); }

};

class mvMenuBar : public mvAppItem
{
public:
    explicit mvMenuBar(mvUUID uuid) : mvAppItem(uuid) { config.height = 21; } // TODO: fix
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_menubar(drawlist, *this); }
};

class mvStage : public mvAppItem
{
public:
    explicit mvStage(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override {}
};

class mvTemplateRegistry : public mvAppItem
{
public:
    explicit mvTemplateRegistry(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override {}
};

class mvClipper : public mvAppItem
{
public:
    explicit mvClipper(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_clipper(drawlist, *this); }

};