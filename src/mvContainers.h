#pragma once

#include "mvItemRegistry.h"
#include <array>

void apply_drag_drop(mvAppItem* item);
void apply_drag_drop_nodraw(mvAppItem* item);

struct mvChildWindowConfig;
struct mvTreeNodeConfig;
struct mvGroupConfig;
struct mvDragPayloadConfig;
struct mvCollapsingHeaderConfig;
struct mvTabBarConfig;
struct mvWindowAppItemConfig;
struct mvMenuConfig;
struct mvTabConfig;

namespace DearPyGui
{
    // specific part of `get_item_configuration(...)`
    void fill_configuration_dict(const mvMenuConfig& inConfig, PyObject* outDict, mvAppItem& item);
    void fill_configuration_dict(const mvTabConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvChildWindowConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvGroupConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvDragPayloadConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvTreeNodeConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvTabBarConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvCollapsingHeaderConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvWindowAppItemConfig& inConfig, PyObject* outDict);

    // specific part of `configure_item(...)`
    void set_configuration(PyObject* inDict, mvMenuConfig& outConfig, mvAppItem& item);
    void set_configuration(PyObject* inDict, mvTabConfig& outConfig);
    void set_configuration(PyObject* inDict, mvChildWindowConfig& outConfig);
    void set_configuration(PyObject* inDict, mvGroupConfig& outConfig);
    void set_configuration(PyObject* inDict, mvDragPayloadConfig& outConfig);
    void set_configuration(PyObject* inDict, mvTreeNodeConfig& outConfig);
    void set_configuration(PyObject* inDict, mvTabBarConfig& outConfig);
    void set_configuration(PyObject* inDict, mvCollapsingHeaderConfig& outConfig);
    void set_configuration(PyObject* inDict, mvAppItem& item, mvWindowAppItemConfig& outConfig);

    // positional args TODO: combine with above
    //void set_required_configuration(PyObject* inDict, mvMenuConfig& outConfig);

    // positional args TODO: combine with above
    //void set_positional_configuration(PyObject* inDict, mvMenuConfig& outConfig);

    // data source handling
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvMenuConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvTabConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvTreeNodeConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvTabBarConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvCollapsingHeaderConfig& outConfig);

    // draw commands
    void draw_menu(ImDrawList* drawlist, mvAppItem& item, mvMenuConfig& config);
    void draw_tab(ImDrawList* drawlist, mvAppItem& item, mvTabConfig& config);
    void draw_child_window(ImDrawList* drawlist, mvAppItem& item, mvChildWindowConfig& config);
    void draw_group(ImDrawList* drawlist, mvAppItem& item, mvGroupConfig& config);
    void draw_drag_payload(ImDrawList* drawlist, mvAppItem& item, mvDragPayloadConfig& config);
    void draw_tree_node(ImDrawList* drawlist, mvAppItem& item, mvTreeNodeConfig& config);
    void draw_tab_bar(ImDrawList* drawlist, mvAppItem& item, mvTabBarConfig& config);
    void draw_collapsing_header(ImDrawList* drawlist, mvAppItem& item, mvCollapsingHeaderConfig& config);
    void draw_window(ImDrawList* drawlist, mvAppItem& item, mvWindowAppItemConfig& config);
}

struct mvMenuConfig
{
    std::shared_ptr<bool> value = std::make_shared<bool>(false);
    bool        _disabled_value = false;
};

struct mvTabConfig
{
    std::shared_ptr<bool>       value = std::make_shared<bool>(false);
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
    ImGuiChildFlags  childFlags = ImGuiChildFlags_Border;
    bool             autosize_x = false;
    bool             autosize_y = false;
    ImGuiWindowFlags windowflags = ImGuiWindowFlags_NoSavedSettings|ImGuiWindowFlags_NavFlattened;
    float            scrollX = 0.0f;
    float            scrollY = 0.0f;
    float            scrollMaxX = 0.0f;
    float            scrollMaxY = 0.0f;
    bool             _scrollXSet = false;
    bool             _scrollYSet = false;
};

struct mvTreeNodeConfig
{
    std::shared_ptr<bool>        value = std::make_shared<bool>(false);
    bool               disabled_value = false;
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
    bool               selectable = false;
};

struct mvGroupConfig
{
    bool  horizontal = false;
    float hspacing = -1.0f;
    float xoffset = 0.0f;
};

struct mvDragPayloadConfig
{
    std::string payloadType = "$$DPG_PAYLOAD";
    PyObject*   dragData = nullptr;
    PyObject*   dropData = nullptr;
};

struct mvCollapsingHeaderConfig
{
    std::shared_ptr<bool>        value = std::make_shared<bool>(false);
    bool               disabled_value = false;
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
    bool               closable = false;
};

struct mvTabBarConfig
{
    std::shared_ptr<mvUUID>    value = std::make_shared<mvUUID>(0);
    mvUUID           disabled_value = 0;
    ImGuiTabBarFlags flags = ImGuiTabBarFlags_None;
    mvUUID           uiValue = 0; // value suggested from UI
    mvUUID           _lastValue = 0;
};

struct mvWindowAppItemConfig
{
    ImGuiWindowFlags windowflags = ImGuiWindowFlags_None;
    bool             mainWindow = false;
    bool             closing = true;
    bool             resized = false;
    bool             modal = false;
    bool             popup = false;
    bool             autosize = false;
    bool             no_resize = false;
    bool             no_title_bar = false;
    bool             no_move = false;
    bool             no_scrollbar = false;
    bool             no_collapse = false;
    bool             horizontal_scrollbar = false;
    bool             no_focus_on_appearing = false;
    bool             no_bring_to_front_on_focus = false;
    bool             menubar = false;
    bool             no_close = false;
    bool             no_background = false;
    bool             collapsed = false;
    bool             unsaved_document = false;
    bool             no_open_over_existing_popup = true;
    PyObject*        on_close = nullptr;
    mvVec2           min_size = { 100.0f, 100.0f };
    mvVec2           max_size = { 30000.0f, 30000.0f };
    float            scrollX = 0.0f;
    float            scrollY = 0.0f;
    float            scrollMaxX = 0.0f;
    float            scrollMaxY = 0.0f;
    bool             _collapsedDirty = true;
    bool             _scrollXSet = false;
    bool             _scrollYSet = false;
    ImGuiWindowFlags _oldWindowflags = ImGuiWindowFlags_None;
    float            _oldxpos = 200;
    float            _oldypos = 200;
    int              _oldWidth = 200;
    int              _oldHeight = 200;
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
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, uuid, configData);}
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
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, uuid, configData); }
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
};

class mvGroup : public mvAppItem
{
public:
    mvGroupConfig configData{};
    explicit mvGroup(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_group(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
};

class mvDragPayload : public mvAppItem
{

public:
    mvDragPayloadConfig configData{};
    explicit mvDragPayload(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_drag_payload(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
};

class mvTreeNode : public mvAppItem
{
public:
    mvTreeNodeConfig configData{};
    explicit mvTreeNode(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_tree_node(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, uuid, configData); }
    PyObject* getPyValue() override { return ToPyBool(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToBool(value); }
};

class mvTabBar : public mvAppItem
{
public:
    mvTabBarConfig configData{};
    explicit mvTabBar(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_tab_bar(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, uuid, configData); }
    PyObject* getPyValue() override{ return ToPyUUID(*configData.value); }
    void setPyValue(PyObject* value) override{ *configData.value = ToUUID(value); }
    mvUUID getSpecificValue() { return configData.uiValue; }
    void setValue(mvUUID value) { configData.uiValue = value; }
};

class mvCollapsingHeader : public mvAppItem
{
public:
    mvCollapsingHeaderConfig configData{};
    explicit mvCollapsingHeader(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_collapsing_header(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, uuid, configData); }
    PyObject* getPyValue() override { return ToPyBool(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToBool(value); }
};

class mvWindowAppItem : public mvAppItem
{
public:
    mvWindowAppItemConfig configData{};
    explicit mvWindowAppItem(mvUUID uuid, bool mainWindow = false) : mvAppItem(uuid) { configData.mainWindow = mainWindow; config.width = config.height = 500; info.dirty_size = true; if (mainWindow) configData.windowflags = ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_window(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, *this, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    ~mvWindowAppItem() { PyObject* callback = configData.on_close; mvSubmitCallback([callback]() { if (callback) Py_XDECREF(callback);});}
};