#pragma once

#include "mvItemRegistry.h"
#include "mvDearPyGui.h"
#include <array>

namespace DearPyGui
{
    // specific part of `get_item_configuration(...)`
    void fill_configuration_dict(const mvMenuConfig& inConfig, PyObject* outDict, mvAppItem& item);
    void fill_configuration_dict(const mvTabConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvChildWindowConfig& inConfig, PyObject* outDict);

    // specific part of `configure_item(...)`
    void set_configuration(PyObject* inDict, mvMenuConfig& outConfig, mvAppItem& item);
    void set_configuration(PyObject* inDict, mvTabConfig& outConfig);
    void set_configuration(PyObject* inDict, mvChildWindowConfig& outConfig);

    // positional args TODO: combine with above
    //void set_required_configuration(PyObject* inDict, mvMenuConfig& outConfig);

    // positional args TODO: combine with above
    //void set_positional_configuration(PyObject* inDict, mvMenuConfig& outConfig);

    // data source handling
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvMenuConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvTabConfig& outConfig);

    // template specifics
    void apply_template(const mvTabConfig& sourceConfig, mvTabConfig& dstConfig);
    void apply_template(const mvChildWindowConfig& sourceConfig, mvChildWindowConfig& dstConfig);

    // draw commands
    void draw_menu(ImDrawList* drawlist, mvAppItem& item, mvMenuConfig& config);
    void draw_tab(ImDrawList* drawlist, mvAppItem& item, mvTabConfig& config);
    void draw_child_window(ImDrawList* drawlist, mvAppItem& item, mvChildWindowConfig& config);
}

struct mvMenuConfig
{
    mvRef<bool> value = CreateRef<bool>(false);
    bool        _disabled_value = false;
};

struct mvTabConfig
{
    mvRef<bool>       value = CreateRef<bool>(false);
    bool              closable = false;
    bool              _disabled_value = false;
    ImGuiTabItemFlags _flags = ImGuiTabItemFlags_None;
};

enum class TabOrdering {
    mvTabOrder_Reorderable = 0L,
    mvTabOrder_Fixed,
    mvTabOrder_Leading,
    mvTabOrder_Trailing
};

struct mvChildWindowConfig
{
    bool             border = true;
    bool             autosize_x = false;
    bool             autosize_y = false;
    ImGuiWindowFlags windowflags = ImGuiWindowFlags_NoSavedSettings;

    // scroll info
    float            scrollX = 0.0f;
    float            scrollY = 0.0f;
    float            scrollMaxX = 0.0f;
    float            scrollMaxY = 0.0f;
    bool             scrollXSet = false;
    bool             scrollYSet = false;
};

//-----------------------------------------------------------------------------
// Old Classes, in the process of removing OOP crap
//-----------------------------------------------------------------------------

class mvMenu : public mvAppItem
{
public:
    mvMenuConfig configData{};
    explicit mvMenu(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_menu(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData, *this); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict, *this); }
    void applySpecificTemplate(mvAppItem* item) override { }
    void setDataSource(mvUUID dataSource) override {}
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyBool(*configData.value); }
    void setPyValue(PyObject* value) override {*configData.value = ToBool(value);}
};

class mvTab : public mvAppItem
{
public:
    mvTabConfig configData{};
    explicit mvTab(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_tab(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvTab*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override {}
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyBool(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToBool(value); }
};

class mvChildWindow : public mvAppItem
{

public:
    mvChildWindowConfig configData{};
    explicit mvChildWindow(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_child_window(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvChildWindow*>(item); DearPyGui::apply_template(titem->configData, configData); }
    //void setScrollX(float value) { _scrollX = value; _scrollXSet = true; }
    //void setScrollY(float value) { _scrollY = value; _scrollYSet = true; }

};
