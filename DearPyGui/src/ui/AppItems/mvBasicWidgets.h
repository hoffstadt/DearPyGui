#pragma once

#include "mvItemRegistry.h"
#include "mvDearPyGui.h"

namespace DearPyGui
{
    // specific part of `get_item_configuration(...)`
    void fill_configuration_dict(const mvButtonConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvComboConfig& inConfig, PyObject* outDict);

    // specific part of `configure_item(...)`
    void set_configuration(PyObject* inDict, mvButtonConfig& outConfig);
    void set_configuration(PyObject* inDict, mvComboConfig& outConfig);

    // positional args TODO: combine with above
    void set_positional_configuration(PyObject* inDict, mvComboConfig& outConfig);

    // data source handling
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvComboConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvCheckboxConfig& outConfig);

    // template specifics
    void apply_template(const mvButtonConfig& sourceConfig, mvButtonConfig& dstConfig);
    void apply_template(const mvComboConfig& sourceConfig, mvComboConfig& dstConfig);
    void apply_template(const mvCheckboxConfig& sourceConfig, mvCheckboxConfig& dstConfig);

    // draw commands
    void draw_button   (ImDrawList* drawlist, mvAppItem& item, const mvButtonConfig& config);
    void draw_combo    (ImDrawList* drawlist, mvAppItem& item, mvComboConfig& config);
    void draw_checkbox (ImDrawList* drawlist, mvAppItem& item, mvCheckboxConfig& config);
    void draw_separator(ImDrawList* drawlist, mvAppItem& item);
    void draw_spacer   (ImDrawList* drawlist, mvAppItem& item);
    void draw_menubar  (ImDrawList* drawlist, mvAppItem& item);
    void draw_clipper  (ImDrawList* drawlist, mvAppItem& item);
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
    std::string              disabled_value = "";
};

struct mvCheckboxConfig
{
    mvRef<bool> value = CreateRef<bool>(false);
    bool        disabled_value = false;
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