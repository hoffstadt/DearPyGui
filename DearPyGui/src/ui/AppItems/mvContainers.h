#pragma once

#include "mvItemRegistry.h"
#include "mvDearPyGui.h"
#include <array>

namespace DearPyGui
{
    // specific part of `get_item_configuration(...)`
    void fill_configuration_dict(const mvMenuConfig& inConfig, PyObject* outDict, mvAppItem& item);

    // specific part of `configure_item(...)`
    void set_configuration(PyObject* inDict, mvMenuConfig& outConfig, mvAppItem& item);

    // positional args TODO: combine with above
    //void set_required_configuration(PyObject* inDict, mvMenuConfig& outConfig);

    // positional args TODO: combine with above
    //void set_positional_configuration(PyObject* inDict, mvMenuConfig& outConfig);

    // data source handling
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvMenuConfig& outConfig);

    // template specifics
    //void apply_template(const mvMenuConfig& sourceConfig, mvMenuConfig& dstConfig);

    // draw commands
    void draw_menu(ImDrawList* drawlist, mvAppItem& item, mvMenuConfig& config);
}

struct mvMenuConfig
{
    mvRef<bool> value = CreateRef<bool>(false);
    bool        _disabled_value = false;
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
