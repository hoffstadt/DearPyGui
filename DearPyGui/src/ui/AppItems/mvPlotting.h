#pragma once

#include "mvItemRegistry.h"
#include "mvDearPyGui.h"

namespace DearPyGui
{
    // specific part of `get_item_configuration(...)`
    void fill_configuration_dict(const mvBarSeriesConfig& inConfig, PyObject* outDict);

    // specific part of `configure_item(...)`
    void set_configuration(PyObject* inDict, mvBarSeriesConfig& outConfig);

    // positional args TODO: combine with above
    void set_required_configuration(PyObject* inDict, mvBarSeriesConfig& outConfig);

    // data source handling
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvBarSeriesConfig& outConfig);

    // template specifics
    void apply_template(const mvBarSeriesConfig& sourceConfig, mvBarSeriesConfig& dstConfig);

    // draw commands
    void draw_bar_series(ImDrawList* drawlist, mvAppItem& item, const mvBarSeriesConfig& config);
}

struct mvBarSeriesConfig
{
    bool horizontal = false;
    float weight = 1.0f;
    mvRef<std::vector<std::vector<double>>> value = CreateRef<std::vector<std::vector<double>>>(
        std::vector<std::vector<double>>{ std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{} });
};

//-----------------------------------------------------------------------------
// Old Classes, in the process of removing OOP crap
//-----------------------------------------------------------------------------

class mvBarSeries : public mvAppItem
{
public:
    mvBarSeriesConfig configData{};
    explicit mvBarSeries(mvUUID uuid) : mvAppItem(uuid) {}
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_required_configuration(dict, configData); }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_bar_series(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvBarSeries*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override{ return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToVectVectDouble(value); }

};