#pragma once

#include "mvItemRegistry.h"
#include "mvDearPyGui.h"
#include <array>

namespace DearPyGui
{
    // specific part of `get_item_configuration(...)`
    void fill_configuration_dict(const mvPlotLegendConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvDragLineConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvDragPointConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvBarSeriesConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvBasicSeriesConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mv2dHistogramSeriesConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvErrorSeriesConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvHeatSeriesConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvHistogramSeriesConfig& inConfig, PyObject* outDict);

    // specific part of `configure_item(...)`
    void set_configuration(PyObject* inDict, mvPlotLegendConfig& outConfig, mvAppItem& item);
    void set_configuration(PyObject* inDict, mvDragLineConfig& outConfig);
    void set_configuration(PyObject* inDict, mvDragPointConfig& outConfig);
    void set_configuration(PyObject* inDict, mvBarSeriesConfig& outConfig);
    void set_configuration(PyObject* inDict, mvBasicSeriesConfig& outConfig);
    void set_configuration(PyObject* inDict, mv2dHistogramSeriesConfig& outConfig);
    void set_configuration(PyObject* inDict, mvErrorSeriesConfig& outConfig);
    void set_configuration(PyObject* inDict, mvHeatSeriesConfig& outConfig);
    void set_configuration(PyObject* inDict, mvHistogramSeriesConfig& outConfig);

    // positional args TODO: combine with above
    void set_required_configuration(PyObject* inDict, mvBarSeriesConfig& outConfig);
    void set_required_configuration(PyObject* inDict, mvBasicSeriesConfig& outConfig);
    void set_required_configuration(PyObject* inDict, mv2dHistogramSeriesConfig& outConfig);
    void set_required_configuration(PyObject* inDict, mvErrorSeriesConfig& outConfig);
    void set_required_configuration(PyObject* inDict, mvHeatSeriesConfig& outConfig);
    void set_required_configuration(PyObject* inDict, mvHistogramSeriesConfig& outConfig);

    // data source handling
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvDragLineConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvDragPointConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvBarSeriesConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvBasicSeriesConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mv2dHistogramSeriesConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvErrorSeriesConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvHeatSeriesConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvHistogramSeriesConfig& outConfig);

    // template specifics
    void apply_template(const mvPlotLegendConfig& sourceConfig, mvPlotLegendConfig& dstConfig);
    void apply_template(const mvDragLineConfig& sourceConfig, mvDragLineConfig& dstConfig);
    void apply_template(const mvDragPointConfig& sourceConfig, mvDragPointConfig& dstConfig);
    void apply_template(const mvBarSeriesConfig& sourceConfig, mvBarSeriesConfig& dstConfig);
    void apply_template(const mvBasicSeriesConfig& sourceConfig, mvBasicSeriesConfig& dstConfig);
    void apply_template(const mv2dHistogramSeriesConfig& sourceConfig, mv2dHistogramSeriesConfig& dstConfig);
    void apply_template(const mvErrorSeriesConfig& sourceConfig, mvErrorSeriesConfig& dstConfig);
    void apply_template(const mvHeatSeriesConfig& sourceConfig, mvHeatSeriesConfig& dstConfig);
    void apply_template(const mvHistogramSeriesConfig& sourceConfig, mvHistogramSeriesConfig& dstConfig);

    // draw commands
    void draw_plot_legend       (ImDrawList* drawlist, mvAppItem& item, mvPlotLegendConfig& config);
    void draw_drag_line         (ImDrawList* drawlist, mvAppItem& item, mvDragLineConfig& config);
    void draw_drag_point        (ImDrawList* drawlist, mvAppItem& item, mvDragPointConfig& config);
    void draw_bar_series        (ImDrawList* drawlist, mvAppItem& item, const mvBarSeriesConfig& config);
    void draw_line_series       (ImDrawList* drawlist, mvAppItem& item, const mvBasicSeriesConfig& config);
    void draw_scatter_series    (ImDrawList* drawlist, mvAppItem& item, const mvBasicSeriesConfig& config);
    void draw_stair_series      (ImDrawList* drawlist, mvAppItem& item, const mvBasicSeriesConfig& config);
    void draw_stem_series       (ImDrawList* drawlist, mvAppItem& item, const mvBasicSeriesConfig& config);
    void draw_shade_series      (ImDrawList* drawlist, mvAppItem& item, const mvBasicSeriesConfig& config);
    void draw_hline_series      (ImDrawList* drawlist, mvAppItem& item, const mvBasicSeriesConfig& config);
    void draw_vline_series      (ImDrawList* drawlist, mvAppItem& item, const mvBasicSeriesConfig& config);
    void draw_2dhistogram_series(ImDrawList* drawlist, mvAppItem& item, const mv2dHistogramSeriesConfig& config);
    void draw_error_series      (ImDrawList* drawlist, mvAppItem& item, const mvErrorSeriesConfig& config);
    void draw_heat_series       (ImDrawList* drawlist, mvAppItem& item, const mvHeatSeriesConfig& config);
    void draw_histogram_series  (ImDrawList* drawlist, mvAppItem& item, const mvHistogramSeriesConfig& config);
}

//-----------------------------------------------------------------------------
// Structs
//-----------------------------------------------------------------------------

struct mvBasicSeriesConfig
{
    mvRef<std::vector<std::vector<double>>> value = CreateRef<std::vector<std::vector<double>>>(
        std::vector<std::vector<double>>{ std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{} });
};

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

struct mvPlotLegendConfig
{
    ImPlotLocation legendLocation = ImPlotLocation_NorthWest;
    bool           horizontal = false;
    bool           outside = false;
    bool           dirty = true;
};

struct mvDragPointConfig
{
    mvRef<std::array<double, 4>> value = CreateRef<std::array<double, 4>>(std::array<double, 4>{0.0, 0.0, 0.0, 0.0});
    double                       disabled_value[4]{};
    bool                         show_label = true;
    mvColor                      color = mvColor(0.0f, 0.0f, 0.0f, -1.0f);
    float                        radius = 4.0f;
};

struct mvDragLineConfig
{
    mvRef<double> value = CreateRef<double>(0.0);
    float         disabled_value = 0.0;
    bool          show_label = true;
    mvColor       color = mvColor(0.0f, 0.0f, 0.0f, -1.0f);
    float         thickness = 1.0f;
    bool          vertical = true;
};

struct mv2dHistogramSeriesConfig
{
    int    xbins = -1;
    int    ybins = -1;
    bool   density = false;
    bool   outliers = true;
    double xmin = 0.0;
    double xmax = 1.0;
    double ymin = 0.0;
    double ymax = 1.0;
    mvRef<std::vector<std::vector<double>>> value = CreateRef<std::vector<std::vector<double>>>(
        std::vector<std::vector<double>>{ std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{} });
};

struct mvErrorSeriesConfig
{
    bool horizontal = false;
    mvRef<std::vector<std::vector<double>>> value = CreateRef<std::vector<std::vector<double>>>(
        std::vector<std::vector<double>>{ std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{} });
};

struct mvHeatSeriesConfig
{
    int         rows = 1;
    int         cols = 1;
    double      scale_min = 0.0;
    double      scale_max = 1.0;
    std::string format = "%0.1f";
    mvPlotPoint bounds_min = { 0.0, 0.0 };
    mvPlotPoint bounds_max = { 1.0, 1.0 };
    mvRef<std::vector<std::vector<double>>> value = CreateRef<std::vector<std::vector<double>>>(
        std::vector<std::vector<double>>{ std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{} });
};

struct mvHistogramSeriesConfig
{
    int    bins = -1;
    bool   cumlative = false;
    bool   density = false;
    bool   outliers = true;
    float  barScale = 1.0f;
    double min = 0.0;
    double max = 1.0;
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

class mvPlotLegend : public mvAppItem
{
public:
    mvPlotLegendConfig configData{};
    explicit mvPlotLegend(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_plot_legend(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData, *this); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvPlotLegend*>(item); DearPyGui::apply_template(titem->configData, configData); }
};

class mvDragLine : public mvAppItem
{
public:
    mvDragLineConfig configData{};
    explicit mvDragLine(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_drag_line(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvDragLine*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyDouble(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToDouble(value); }
};

class mvDragPoint : public mvAppItem
{
public:
    mvDragPointConfig configData{};
    explicit mvDragPoint(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_drag_point(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvDragPoint*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyFloatList(configData.value->data(), 4); }
    void setPyValue(PyObject* value) override;
};

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

class mvLineSeries : public mvAppItem
{
public:
    mvBasicSeriesConfig configData{};
    explicit mvLineSeries(mvUUID uuid) : mvAppItem(uuid) {}
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_required_configuration(dict, configData); }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_line_series(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvLineSeries*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToVectVectDouble(value); }
};

class mvScatterSeries : public mvAppItem
{
public:
    mvBasicSeriesConfig configData{};
    explicit mvScatterSeries(mvUUID uuid) : mvAppItem(uuid) {}
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_required_configuration(dict, configData); }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_scatter_series(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvScatterSeries*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToVectVectDouble(value); }
};

class mvShadeSeries : public mvAppItem
{
public:
    mvBasicSeriesConfig configData{};
    explicit mvShadeSeries(mvUUID uuid) : mvAppItem(uuid) {}
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_required_configuration(dict, configData); }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_shade_series(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvShadeSeries*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToVectVectDouble(value); }
};

class mvVLineSeries : public mvAppItem
{
public:
    mvBasicSeriesConfig configData{};
    explicit mvVLineSeries(mvUUID uuid) : mvAppItem(uuid) {}
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_required_configuration(dict, configData); }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_vline_series(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvVLineSeries*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToVectVectDouble(value); }
};

class mvHLineSeries : public mvAppItem
{
public:
    mvBasicSeriesConfig configData{};
    explicit mvHLineSeries(mvUUID uuid) : mvAppItem(uuid) {}
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_required_configuration(dict, configData); }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_hline_series(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvHLineSeries*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToVectVectDouble(value); }
};

class mvStairSeries : public mvAppItem
{
public:
    mvBasicSeriesConfig configData{};
    explicit mvStairSeries(mvUUID uuid) : mvAppItem(uuid) {}
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_required_configuration(dict, configData); }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_stair_series(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvStairSeries*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToVectVectDouble(value); }
};

class mvStemSeries : public mvAppItem
{
public:
    mvBasicSeriesConfig configData{};
    explicit mvStemSeries(mvUUID uuid) : mvAppItem(uuid) {}
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_required_configuration(dict, configData); }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_stem_series(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvStemSeries*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToVectVectDouble(value); }
};

class mv2dHistogramSeries : public mvAppItem
{
public:
    mv2dHistogramSeriesConfig configData{};
    explicit mv2dHistogramSeries(mvUUID uuid) : mvAppItem(uuid) {}
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_required_configuration(dict, configData); }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_2dhistogram_series(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mv2dHistogramSeries*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToVectVectDouble(value); }
};

class mvErrorSeries : public mvAppItem
{
public:
    mvErrorSeriesConfig configData{};
    explicit mvErrorSeries(mvUUID uuid) : mvAppItem(uuid) {}
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_required_configuration(dict, configData); }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_error_series(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvErrorSeries*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToVectVectDouble(value); }
};

class mvHeatSeries : public mvAppItem
{
public:
    mvHeatSeriesConfig configData{};
    explicit mvHeatSeries(mvUUID uuid) : mvAppItem(uuid) {}
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_required_configuration(dict, configData); }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_heat_series(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvHeatSeries*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToVectVectDouble(value); }
};

class mvHistogramSeries : public mvAppItem
{
public:
    mvHistogramSeriesConfig configData{};
    explicit mvHistogramSeries(mvUUID uuid) : mvAppItem(uuid) {}
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_required_configuration(dict, configData); }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_histogram_series(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvHistogramSeries*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToVectVectDouble(value); }
};