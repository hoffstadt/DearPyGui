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
    void fill_configuration_dict(const mvPieSeriesConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvLabelSeriesConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvImageSeriesConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvAreaSeriesConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvCandleSeriesConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvCustomSeriesConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvAnnotationConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvSubPlotsConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvPlotAxisConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvPlotConfig& inConfig, PyObject* outDict);

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
    void set_configuration(PyObject* inDict, mvPieSeriesConfig& outConfig);
    void set_configuration(PyObject* inDict, mvLabelSeriesConfig& outConfig);
    void set_configuration(PyObject* inDict, mvImageSeriesConfig& outConfig);
    void set_configuration(PyObject* inDict, mvAreaSeriesConfig& outConfig);
    void set_configuration(PyObject* inDict, mvCandleSeriesConfig& outConfig);
    void set_configuration(PyObject* inDict, mvCustomSeriesConfig& outConfig);
    void set_configuration(PyObject* inDict, mvAnnotationConfig& outConfig);
    void set_configuration(PyObject* inDict, mvSubPlotsConfig& outConfig);
    void set_configuration(PyObject* inDict, mvPlotAxisConfig& outConfig, mvAppItem& item);
    void set_configuration(PyObject* inDict, mvPlotConfig& outConfig);

    // positional args TODO: combine with above
    void set_positional_configuration(PyObject* inDict, mvBarSeriesConfig& outConfig);
    void set_positional_configuration(PyObject* inDict, mvBasicSeriesConfig& outConfig);
    void set_positional_configuration(PyObject* inDict, mv2dHistogramSeriesConfig& outConfig);
    void set_positional_configuration(PyObject* inDict, mvErrorSeriesConfig& outConfig);
    void set_positional_configuration(PyObject* inDict, mvHeatSeriesConfig& outConfig);
    void set_positional_configuration(PyObject* inDict, mvHistogramSeriesConfig& outConfig);
    void set_positional_configuration(PyObject* inDict, mvPieSeriesConfig& outConfig);
    void set_positional_configuration(PyObject* inDict, mvLabelSeriesConfig& outConfig);

    // required args
    void set_required_configuration(PyObject* inDict, mvSubPlotsConfig& outConfig);
    void set_required_configuration(PyObject* inDict, mvImageSeriesConfig& outConfig);
    void set_required_configuration(PyObject* inDict, mvAreaSeriesConfig& outConfig);
    void set_required_configuration(PyObject* inDict, mvCandleSeriesConfig& outConfig);
    void set_required_configuration(PyObject* inDict, mvCustomSeriesConfig& outConfig);
    void set_required_configuration(PyObject* inDict, mvPlotAxisConfig& outConfig);

    // data source handling
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvAnnotationConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvDragLineConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvDragPointConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvRef<std::vector<std::vector<double>>>& outValue);

    // template specifics
    void apply_template(const mvSubPlotsConfig& sourceConfig, mvSubPlotsConfig& dstConfig);
    void apply_template(const mvPlotLegendConfig& sourceConfig, mvPlotLegendConfig& dstConfig);
    void apply_template(const mvDragLineConfig& sourceConfig, mvDragLineConfig& dstConfig);
    void apply_template(const mvDragPointConfig& sourceConfig, mvDragPointConfig& dstConfig);
    void apply_template(const mvBarSeriesConfig& sourceConfig, mvBarSeriesConfig& dstConfig);
    void apply_template(const mvBasicSeriesConfig& sourceConfig, mvBasicSeriesConfig& dstConfig);
    void apply_template(const mv2dHistogramSeriesConfig& sourceConfig, mv2dHistogramSeriesConfig& dstConfig);
    void apply_template(const mvErrorSeriesConfig& sourceConfig, mvErrorSeriesConfig& dstConfig);
    void apply_template(const mvHeatSeriesConfig& sourceConfig, mvHeatSeriesConfig& dstConfig);
    void apply_template(const mvHistogramSeriesConfig& sourceConfig, mvHistogramSeriesConfig& dstConfig);
    void apply_template(const mvPieSeriesConfig& sourceConfig, mvPieSeriesConfig& dstConfig);
    void apply_template(const mvLabelSeriesConfig& sourceConfig, mvLabelSeriesConfig& dstConfig);
    void apply_template(const mvImageSeriesConfig& sourceConfig, mvImageSeriesConfig& dstConfig);
    void apply_template(const mvAreaSeriesConfig& sourceConfig, mvAreaSeriesConfig& dstConfig);
    void apply_template(const mvCandleSeriesConfig& sourceConfig, mvCandleSeriesConfig& dstConfig);
    void apply_template(const mvCustomSeriesConfig& sourceConfig, mvCustomSeriesConfig& dstConfig);
    void apply_template(const mvAnnotationConfig& sourceConfig, mvAnnotationConfig& dstConfig);
    void apply_template(const mvPlotAxisConfig& sourceConfig, mvPlotAxisConfig& dstConfig);
    void apply_template(const mvPlotConfig& sourceConfig, mvPlotConfig& dstConfig);

    // draw commands
    void draw_plot              (ImDrawList* drawlist, mvAppItem& item, mvPlotConfig& config);
    void draw_plot_axis         (ImDrawList* drawlist, mvAppItem& item, mvPlotAxisConfig& config);
    void draw_subplots          (ImDrawList* drawlist, mvAppItem& item, mvSubPlotsConfig& config);
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
    void draw_pie_series        (ImDrawList* drawlist, mvAppItem& item, const mvPieSeriesConfig& config);
    void draw_label_series      (ImDrawList* drawlist, mvAppItem& item, const mvLabelSeriesConfig& config);
    void draw_image_series      (ImDrawList* drawlist, mvAppItem& item, mvImageSeriesConfig& config);
    void draw_area_series       (ImDrawList* drawlist, mvAppItem& item, const mvAreaSeriesConfig& config);
    void draw_candle_series     (ImDrawList* drawlist, mvAppItem& item, const mvCandleSeriesConfig& config);
    void draw_custom_series     (ImDrawList* drawlist, mvAppItem& item, mvCustomSeriesConfig& config);
    void draw_plot_annotation   (ImDrawList* drawlist, mvAppItem& item, mvAnnotationConfig& config);
}

//-----------------------------------------------------------------------------
// Structs
//-----------------------------------------------------------------------------

struct mvBasicSeriesConfig
{
    mvAppItemType type = mvAppItemType::All;
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

struct mvPieSeriesConfig
{
    double                   x = 0.0;
    double                   y = 0.0;
    double                   radius = 0.5;
    bool                     normalize = true;
    double                   angle = 90.0;
    std::string              format;
    std::vector<std::string> labels;
    std::vector<const char*> clabels;
    mvRef<std::vector<std::vector<double>>> value = CreateRef<std::vector<std::vector<double>>>(
        std::vector<std::vector<double>>{ std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{} });
};

struct mvLabelSeriesConfig
{
    int  xoffset = 0;
    int  yoffset = 0;
    bool vertical = false;
    mvRef<std::vector<std::vector<double>>> value = CreateRef<std::vector<std::vector<double>>>(
        std::vector<std::vector<double>>{ std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{} });
};

struct mvImageSeriesConfig
{
    // config
    mvUUID      textureUUID = 0;
    mvPlotPoint bounds_min = { 0.0, 0.0 };
    mvPlotPoint bounds_max = { 0.0, 0.0 };
    mvVec2      uv_min = { 0.0f, 0.0f };
    mvVec2      uv_max = { 1.0f, 1.0f };
    mvColor     tintColor = { 1.0f, 1.0f, 1.0f, 1.0f };

    // pointer to existing item or internal
    std::shared_ptr<mvAppItem> _texture = nullptr;
    bool _internalTexture = false; // create a local texture if necessary

    mvRef<std::vector<std::vector<double>>> value = CreateRef<std::vector<std::vector<double>>>(
        std::vector<std::vector<double>>{ std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{} });
};

struct mvAreaSeriesConfig
{
    mvColor fill = MV_DEFAULT_COLOR;
    mvRef<std::vector<std::vector<double>>> value = CreateRef<std::vector<std::vector<double>>>(
        std::vector<std::vector<double>>{ std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{} });
};

struct mvCandleSeriesConfig
{
    float   weight = 0.25f;
    bool    tooltip = true;
    int     timeunit = ImPlotTimeUnit_Day;
    mvColor bullColor = { 0, 255, 113, 255 };
    mvColor bearColor = { 218, 13, 79, 255 };
    mvRef<std::vector<std::vector<double>>> value = CreateRef<std::vector<std::vector<double>>>(
        std::vector<std::vector<double>>{ std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{} });
};

struct mvCustomSeriesConfig
{
    int channelCount = 2; // must be between 2 and 5 inclusive
    bool tooltip = true;
    mvRef<std::vector<std::vector<double>>> value = CreateRef<std::vector<std::vector<double>>>(
        std::vector<std::vector<double>>{ std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{} });
    std::vector<std::vector<double>> _transformedValues;
};

struct mvAnnotationConfig
{
    mvRef<std::array<double, 4>> value = CreateRef<std::array<double, 4>>(std::array<double, 4>{0.0, 0.0, 0.0, 0.0});
    double                       disabled_value[4]{};
    mvColor                      color = mvColor(0.0f, 0.0f, 0.0f, -1.0f);
    bool                         clamped = true;
    ImVec2                       pixOffset;
};

struct mvSubPlotsConfig
{
    int                rows = 1;
    int                cols = 1;
    std::vector<float> row_ratios;
    std::vector<float> col_ratios;
    ImPlotSubplotFlags flags = ImPlotSubplotFlags_None;
};

struct mvPlotAxisConfig
{
    ImPlotAxisFlags          flags = 0;
    int                      axis = 0;
    bool                     setLimits = false;
    ImVec2                   limits;
    ImVec2                   limits_actual;
    std::vector<std::string> labels;
    std::vector<double>      labelLocations;
    std::vector<const char*> clabels; // to prevent conversion from string to char* every frame
    bool                     _dirty = false;
};

struct mvPlotConfig
{
    std::string     xaxisName;
    int             pan_button = 0;
    int             pan_mod = 0;
    int             fit_button = 0;
    int             context_menu_button = 0;
    int             box_select_button = 0;
    int             box_select_mod = 0;
    int             box_select_cancel_button = 0;
    int             query_button = 0;
    int             query_mod = 0;
    int             query_toggle_mod = 0;
    int             horizontal_mod = 0;
    int             vertical_mod = 0;

    std::string     _y1axisName;
    std::string     _y2axisName;
    std::string     _y3axisName;
    ImPlotFlags     _flags = ImPlotFlags_NoLegend;
    ImPlotAxisFlags _xflags = 0;
    ImPlotAxisFlags _yflags = 0;
    ImPlotAxisFlags _y1flags = 0;
    ImPlotAxisFlags _y2flags = 0;
    bool            _newColorMap = false; // to bust color cache
    bool            _useColorMap = false;
    ImPlotColormap  _colormap = ImPlotColormap_Deep;
    bool            _equalAspectRatios = false;
    bool            _queried = false;
    double          _queryArea[4] = { 0.0, 0.0, 0.0, 0.0 };
    bool            _fitDirty = false;
    bool            _axisfitDirty[4] = { false, false, false, false }; 
    ImPlotInputMap  _originalMap = ImPlotInputMap();// custom input mapping
};

//-----------------------------------------------------------------------------
// Old Classes, in the process of removing OOP crap
//-----------------------------------------------------------------------------

class mvSubPlots : public mvAppItem
{
public:
    mvSubPlotsConfig configData{};
    explicit mvSubPlots(mvUUID uuid) : mvAppItem(uuid) { config.width = config.height = -1; }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_subplots(drawlist, *this, configData); }
    void handleSpecificRequiredArgs(PyObject* args) override { DearPyGui::set_required_configuration(args, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvSubPlots*>(item); DearPyGui::apply_template(titem->configData, configData); }
    inline void onChildRemoved(mvRef<mvAppItem> item) { if (item->type == mvAppItemType::mvPlotLegend){ configData.flags |= ImPlotSubplotFlags_NoLegend; configData.flags &= ~ImPlotSubplotFlags_ShareItems;}}
    inline void onChildAdd(mvRef<mvAppItem> item) { if (item->type == mvAppItemType::mvPlotLegend) { configData.flags &= ~ImPlotSubplotFlags_NoLegend; configData.flags |= ImPlotSubplotFlags_ShareItems;}}
    inline void addFlag(ImPlotSubplotFlags flag) { configData.flags |= flag; }
    inline void removeFlag(ImPlotSubplotFlags flag){ configData.flags &= ~flag; }
};

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
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_positional_configuration(dict, configData); }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_bar_series(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvBarSeries*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData.value); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override{ return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToVectVectDouble(value); }
};

class mvLineSeries : public mvAppItem
{
public:
    mvBasicSeriesConfig configData{};
    explicit mvLineSeries(mvUUID uuid) : mvAppItem(uuid) { configData.type = mvAppItemType::mvLineSeries; }
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_positional_configuration(dict, configData); }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_line_series(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvLineSeries*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData.value); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToVectVectDouble(value); }
};

class mvScatterSeries : public mvAppItem
{
public:
    mvBasicSeriesConfig configData{ };
    explicit mvScatterSeries(mvUUID uuid) : mvAppItem(uuid) { configData.type = mvAppItemType::mvScatterSeries; }
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_positional_configuration(dict, configData); }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_scatter_series(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvScatterSeries*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData.value); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToVectVectDouble(value); }
};

class mvShadeSeries : public mvAppItem
{
public:
    mvBasicSeriesConfig configData{};
    explicit mvShadeSeries(mvUUID uuid) : mvAppItem(uuid) { configData.type = mvAppItemType::mvShadeSeries; }
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_positional_configuration(dict, configData); }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_shade_series(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvShadeSeries*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData.value); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToVectVectDouble(value); }
};

class mvVLineSeries : public mvAppItem
{
public:
    mvBasicSeriesConfig configData{};
    explicit mvVLineSeries(mvUUID uuid) : mvAppItem(uuid) { configData.type = mvAppItemType::mvVLineSeries; }
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_positional_configuration(dict, configData); }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_vline_series(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvVLineSeries*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData.value); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToVectVectDouble(value); }
};

class mvHLineSeries : public mvAppItem
{
public:
    mvBasicSeriesConfig configData{};
    explicit mvHLineSeries(mvUUID uuid) : mvAppItem(uuid) { configData.type = mvAppItemType::mvHLineSeries; }
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_positional_configuration(dict, configData); }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_hline_series(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvHLineSeries*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData.value); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToVectVectDouble(value); }
};

class mvStairSeries : public mvAppItem
{
public:
    mvBasicSeriesConfig configData{};
    explicit mvStairSeries(mvUUID uuid) : mvAppItem(uuid) { configData.type = mvAppItemType::mvStairSeries; }
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_positional_configuration(dict, configData); }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_stair_series(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvStairSeries*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData.value); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToVectVectDouble(value); }
};

class mvStemSeries : public mvAppItem
{
public:
    mvBasicSeriesConfig configData{};
    explicit mvStemSeries(mvUUID uuid) : mvAppItem(uuid) { configData.type = mvAppItemType::mvStemSeries; }
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_positional_configuration(dict, configData); }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_stem_series(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvStemSeries*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData.value); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToVectVectDouble(value); }
};

class mv2dHistogramSeries : public mvAppItem
{
public:
    mv2dHistogramSeriesConfig configData{};
    explicit mv2dHistogramSeries(mvUUID uuid) : mvAppItem(uuid) {}
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_positional_configuration(dict, configData); }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_2dhistogram_series(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mv2dHistogramSeries*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData.value); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToVectVectDouble(value); }
};

class mvErrorSeries : public mvAppItem
{
public:
    mvErrorSeriesConfig configData{};
    explicit mvErrorSeries(mvUUID uuid) : mvAppItem(uuid) {}
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_positional_configuration(dict, configData); }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_error_series(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvErrorSeries*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData.value); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToVectVectDouble(value); }
};

class mvHeatSeries : public mvAppItem
{
public:
    mvHeatSeriesConfig configData{};
    explicit mvHeatSeries(mvUUID uuid) : mvAppItem(uuid) {}
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_positional_configuration(dict, configData); }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_heat_series(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvHeatSeries*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData.value); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToVectVectDouble(value); }
};

class mvHistogramSeries : public mvAppItem
{
public:
    mvHistogramSeriesConfig configData{};
    explicit mvHistogramSeries(mvUUID uuid) : mvAppItem(uuid) {}
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_positional_configuration(dict, configData); }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_histogram_series(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvHistogramSeries*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData.value); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToVectVectDouble(value); }
};

class mvPieSeries : public mvAppItem
{
public:
    mvPieSeriesConfig configData{};
    explicit mvPieSeries(mvUUID uuid) : mvAppItem(uuid) {}
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_positional_configuration(dict, configData); }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_pie_series(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvPieSeries*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData.value); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToVectVectDouble(value); }
};

class mvLabelSeries : public mvAppItem
{
public:
    mvLabelSeriesConfig configData{};
    explicit mvLabelSeries(mvUUID uuid) : mvAppItem(uuid) {}
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_positional_configuration(dict, configData); }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_label_series(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvLabelSeries*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData.value); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToVectVectDouble(value); }
};

class mvImageSeries : public mvAppItem
{
public:
    mvImageSeriesConfig configData{};
    explicit mvImageSeries(mvUUID uuid) : mvAppItem(uuid) {}
    void handleSpecificRequiredArgs(PyObject* dict) override { DearPyGui::set_required_configuration(dict, configData); }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_image_series(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvImageSeries*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData.value); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToVectVectDouble(value); }
};

class mvAreaSeries : public mvAppItem
{
public:
    mvAreaSeriesConfig configData{};
    explicit mvAreaSeries(mvUUID uuid) : mvAppItem(uuid) {}
    void handleSpecificRequiredArgs(PyObject* dict) override { DearPyGui::set_required_configuration(dict, configData); }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_area_series(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvAreaSeries*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData.value); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToVectVectDouble(value); }
};

class mvCandleSeries : public mvAppItem
{
public:
    mvCandleSeriesConfig configData{};
    explicit mvCandleSeries(mvUUID uuid) : mvAppItem(uuid) {}
    void handleSpecificRequiredArgs(PyObject* dict) override { DearPyGui::set_required_configuration(dict, configData); }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_candle_series(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvCandleSeries*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData.value); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToVectVectDouble(value); }
};

class mvCustomSeries : public mvAppItem
{
public:
    mvCustomSeriesConfig configData{};
    explicit mvCustomSeries(mvUUID uuid) : mvAppItem(uuid) {}
    void handleSpecificRequiredArgs(PyObject* dict) override { DearPyGui::set_required_configuration(dict, configData); }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_custom_series(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvCustomSeries*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData.value); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToVectVectDouble(value); }
};

class mvAnnotation : public mvAppItem
{
public:
    mvAnnotationConfig configData{};
    explicit mvAnnotation(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_plot_annotation(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvAnnotation*>(item); DearPyGui::apply_template(titem->configData, configData); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return +ToPyFloatList(configData.value->data(), 4); }
    void setPyValue(PyObject* value) override;
};

class mvPlotAxis : public mvAppItem
{
public:
    mvPlotAxisConfig configData{};
    explicit mvPlotAxis(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_plot_axis(drawlist, *this, configData); }
    void handleSpecificRequiredArgs(PyObject* dict) override { DearPyGui::set_required_configuration(dict, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData, *this); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvPlotAxis*>(item); DearPyGui::apply_template(titem->configData, configData); }
};

class mvPlot : public mvAppItem
{
public:
    mvPlotConfig configData{};
    explicit mvPlot(mvUUID uuid) : mvAppItem(uuid) { config.width = config.height = -1; }
    void updateFlags();
    void updateAxesNames();
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_plot(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void applySpecificTemplate(mvAppItem* item) override { auto titem = static_cast<mvPlot*>(item); DearPyGui::apply_template(titem->configData, configData); }
};