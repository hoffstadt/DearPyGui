#pragma once

#include "mvItemRegistry.h"
#include <array>

struct mvPlotConfig;
struct mvPlotAxisConfig;
struct mvAnnotationConfig;
struct mvSubPlotsConfig;
struct mvPlotLegendConfig;
struct mvDragLineConfig;
struct mvDragRectConfig;
struct mvDragPointConfig;
struct mvBasicSeriesConfig;
struct mvLineSeriesConfig;
struct mvBarSeriesConfig;
struct mvStairSeriesConfig;
struct mvInfLineSeriesConfig;
struct mvScatterSeriesConfig;
struct mv2dHistogramSeriesConfig;
struct mvHistogramSeriesConfig;
struct mvErrorSeriesConfig;
struct mvHeatSeriesConfig;
struct mvPieSeriesConfig;
struct mvLabelSeriesConfig;
struct mvImageSeriesConfig;
struct mvAreaSeriesConfig;
struct mvCandleSeriesConfig;
struct mvCustomSeriesConfig;

namespace DearPyGui
{
    // specific part of `get_item_configuration(...)`
    void fill_configuration_dict(const mvPlotLegendConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvDragLineConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvDragRectConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvDragPointConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvLineSeriesConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvBarSeriesConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvStairSeriesConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvInfLineSeriesConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvScatterSeriesConfig& inConfig, PyObject* outDict);
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
    void set_configuration(PyObject* inDict, mvDragRectConfig& outConfig);
    void set_configuration(PyObject* inDict, mvDragPointConfig& outConfig);
    void set_configuration(PyObject* inDict, mvLineSeriesConfig& outConfig);
    void set_configuration(PyObject* inDict, mvBarSeriesConfig& outConfig);
    void set_configuration(PyObject* inDict, mvStairSeriesConfig& outConfig);
    void set_configuration(PyObject* inDict, mvInfLineSeriesConfig& outConfig);
    void set_configuration(PyObject* inDict, mvScatterSeriesConfig& outConfig);
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
    void set_positional_configuration(PyObject* inDict, mvLineSeriesConfig& outConfig);
    void set_positional_configuration(PyObject* inDict, mvBarSeriesConfig& outConfig);
    void set_positional_configuration(PyObject* inDict, mvStairSeriesConfig& outConfig);
    void set_positional_configuration(PyObject* inDict, mvInfLineSeriesConfig& outConfig);
    void set_positional_configuration(PyObject* inDict, mvScatterSeriesConfig& outConfig);
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
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvDragRectConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvDragPointConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, std::shared_ptr<std::vector<std::vector<double>>>& outValue);

    // draw commands
    void draw_plot              (ImDrawList* drawlist, mvAppItem& item, mvPlotConfig& config);
    void draw_plot_axis         (ImDrawList* drawlist, mvAppItem& item, mvPlotAxisConfig& config);
    void draw_subplots          (ImDrawList* drawlist, mvAppItem& item, mvSubPlotsConfig& config);
    void draw_plot_legend       (ImDrawList* drawlist, mvAppItem& item, mvPlotLegendConfig& config);
    void draw_drag_line         (ImDrawList* drawlist, mvAppItem& item, mvDragLineConfig& config);
    void draw_drag_rect         (ImDrawList* drawlist, mvAppItem& item, mvDragRectConfig& config);
    void draw_drag_point        (ImDrawList* drawlist, mvAppItem& item, mvDragPointConfig& config);
    void draw_bar_series        (ImDrawList* drawlist, mvAppItem& item, const mvBarSeriesConfig& config);
    void draw_inf_lines_series  (ImDrawList* drawlist, mvAppItem& item, const mvInfLineSeriesConfig& config);
    void draw_line_series       (ImDrawList* drawlist, mvAppItem& item, const mvLineSeriesConfig& config);
    void draw_scatter_series    (ImDrawList* drawlist, mvAppItem& item, const mvScatterSeriesConfig& config);
    void draw_stair_series      (ImDrawList* drawlist, mvAppItem& item, const mvStairSeriesConfig& config);
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
    std::shared_ptr<std::vector<std::vector<double>>> value = std::make_shared<std::vector<std::vector<double>>>(
        std::vector<std::vector<double>>{ std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{} });
};

struct mvLineSeriesConfig
{
    ImPlotLineFlags flags = ImPlotLineFlags_None;
    std::shared_ptr<std::vector<std::vector<double>>> value = std::make_shared<std::vector<std::vector<double>>>(
        std::vector<std::vector<double>>{ std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{} });
};

struct mvBarSeriesConfig
{
    ImPlotBarsFlags flags = ImPlotBarsFlags_None;
    float weight = 1.0f;
    std::shared_ptr<std::vector<std::vector<double>>> value = std::make_shared<std::vector<std::vector<double>>>(
        std::vector<std::vector<double>>{ std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{} });
};

struct mvStairSeriesConfig
{
    ImPlotStairsFlags flags = ImPlotStairsFlags_None;
    std::shared_ptr<std::vector<std::vector<double>>> value = std::make_shared<std::vector<std::vector<double>>>(
        std::vector<std::vector<double>>{ std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{} });
};

struct mvInfLineSeriesConfig
{
    ImPlotInfLinesFlags flags = ImPlotInfLinesFlags_None;
    std::shared_ptr<std::vector<std::vector<double>>> value = std::make_shared<std::vector<std::vector<double>>>(
        std::vector<std::vector<double>>{ std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{} });
};

struct mvScatterSeriesConfig
{
    ImPlotScatterFlags flags = ImPlotScatterFlags_None;
    std::shared_ptr<std::vector<std::vector<double>>> value = std::make_shared<std::vector<std::vector<double>>>(
        std::vector<std::vector<double>>{ std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{} });
};

struct mvPlotLegendConfig
{
    ImPlotLocation      legendLocation = ImPlotLocation_NorthWest;
    ImPlotLegendFlags   flags = ImPlotLegendFlags_None;
    bool                dirty = true;
};

struct _mvDragItem {
    ImPlotDragToolFlags          flags = ImPlotDragToolFlags_None;
    mvColor                      color = mvColor(0.0f, 0.0f, 0.0f, -1.0f);
};

struct mvDragPointConfig : _mvDragItem
{
    std::shared_ptr<std::array<double, 2>> value = std::make_shared<std::array<double, 2>>(std::array<double, 2>{0.0, 0.0});
    double                       disabled_value[2]{};
    float                        radius = 4.0f;
};

struct mvDragRectConfig : _mvDragItem
{
    std::shared_ptr<std::array<double, 4>> value = std::make_shared<std::array<double, 4>>(std::array<double, 4>{0.0, 0.0, 0.0, 0.0});
    double                       disabled_value[4]{};
};

struct mvDragLineConfig : _mvDragItem
{
    std::shared_ptr<double> value = std::make_shared<double>(0.0);
    float               disabled_value = 0.0;
    float               thickness = 1.0f;
    bool                vertical = true;
};

struct mv2dHistogramSeriesConfig
{
    int    xbins = -1;
    int    ybins = -1;
    ImPlotHistogramFlags flags = ImPlotHistogramFlags_None;
    double xmin = 0.0;
    double xmax = 1.0;
    double ymin = 0.0;
    double ymax = 1.0;
    std::shared_ptr<std::vector<std::vector<double>>> value = std::make_shared<std::vector<std::vector<double>>>(
        std::vector<std::vector<double>>{ std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{} });
};

struct mvErrorSeriesConfig
{
    ImPlotErrorBarsFlags flags = ImPlotErrorBarsFlags_None;
    std::shared_ptr<std::vector<std::vector<double>>> value = std::make_shared<std::vector<std::vector<double>>>(
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
    ImPlotPoint bounds_min = { 0.0, 0.0 };
    ImPlotPoint bounds_max = { 1.0, 1.0 };
    std::shared_ptr<std::vector<std::vector<double>>> value = std::make_shared<std::vector<std::vector<double>>>(
        std::vector<std::vector<double>>{ std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{} });
};

struct mvHistogramSeriesConfig
{
    int    bins = -1;
    ImPlotHistogramFlags flags = ImPlotHistogramFlags_None;
    float  barScale = 1.0f;
    double min = 0.0;
    double max = 1.0;
    std::shared_ptr<std::vector<std::vector<double>>> value = std::make_shared<std::vector<std::vector<double>>>(
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
    ImPlotPieChartFlags      flags = ImPlotPieChartFlags_None;
    double                   angle = 90.0;
    std::string              format;
    std::vector<std::string> labels;
    std::vector<const char*> clabels;
    std::shared_ptr<std::vector<std::vector<double>>> value = std::make_shared<std::vector<std::vector<double>>>(
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
    ImPlotTextFlags flags = ImPlotTextFlags_None;
    std::shared_ptr<std::vector<std::vector<double>>> value = std::make_shared<std::vector<std::vector<double>>>(
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
    ImPlotPoint bounds_min = { 0.0, 0.0 };
    ImPlotPoint bounds_max = { 0.0, 0.0 };
    mvVec2      uv_min = { 0.0f, 0.0f };
    mvVec2      uv_max = { 1.0f, 1.0f };
    mvColor     tintColor = { 1.0f, 1.0f, 1.0f, 1.0f };

    // pointer to existing item or internal
    std::shared_ptr<mvAppItem> _texture = nullptr;
    bool _internalTexture = false; // create a local texture if necessary

    std::shared_ptr<std::vector<std::vector<double>>> value = std::make_shared<std::vector<std::vector<double>>>(
        std::vector<std::vector<double>>{ std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{} });
};

struct mvAreaSeriesConfig
{
    mvColor fill = MV_DEFAULT_COLOR;
    std::shared_ptr<std::vector<std::vector<double>>> value = std::make_shared<std::vector<std::vector<double>>>(
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
    std::shared_ptr<std::vector<std::vector<double>>> value = std::make_shared<std::vector<std::vector<double>>>(
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
    ImPlotItemFlags flags = ImPlotItemFlags_None;
    std::shared_ptr<std::vector<std::vector<double>>> value = std::make_shared<std::vector<std::vector<double>>>(
        std::vector<std::vector<double>>{ std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{},
        std::vector<double>{} });
    std::vector<std::vector<double>> _transformedValues;
};

struct mvAnnotationConfig
{
    std::shared_ptr<std::array<double, 4>> value = std::make_shared<std::array<double, 4>>(std::array<double, 4>{0.0, 0.0, 0.0, 0.0});
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
    ImPlotAxisFlags          flags = ImPlotAxisFlags_None;
    ImAxis                   axis = ImAxis_X1;
    bool                     setLimits = false;
    ImPlotScale              scale = ImPlotScale_Linear;
    std::string              formatter;
    ImVec2                   limits;
    ImVec2                   limits_actual;
    ImVec2                   constraints_range;
    ImVec2                   zoom_range;
    std::vector<std::string> labels;
    std::vector<double>      labelLocations;
    std::vector<const char*> clabels; // to prevent conversion from string to char* every frame
    bool                     _dirty = false;
};

struct mvPlotConfig
{
    // TODO: Maybe they must be all initialized here
    ImGuiMouseButton             pan = 0;       // LMB    enables panning when held,
    ImGuiKey                     pan_mod = ImGuiKey_None;   // none   optional modifier that must be held for panning/fitting
    ImGuiMouseButton             fit;           // LMB    initiates fit when double clicked
    ImGuiMouseButton             menu;          // RMB    opens context menus (if enabled) when clicked
    ImGuiMouseButton             select;        // RMB    begins box selection when pressed and confirms selection when released
    ImGuiKey                     select_mod = ImGuiKey_None;     // none   optional modifier that must be held for box selection
    ImGuiMouseButton             select_cancel;  // LMB    cancels active box selection when pressed; cannot be same as Select
    int             query_button = 0;
    int             query_mod = 0;  // TODO: Check if these variables are used, then delete them
    int             query_toggle_mod = 0;
    ImGuiKey                     select_horz_mod = ImGuiMod_Alt; // Alt    expands active box selection horizontally to plot edge when held
    ImGuiKey                     select_vert_mod = ImGuiMod_Shift; // Shift  expands active box selection vertically to plot edge when held

    // Input map
    ImGuiKey                     override_mod = ImGuiMod_Ctrl;   // Ctrl   when held, all input is ignored; used to enable axis/plots as DND sources
    ImGuiKey                     zoom_mod = ImGuiKey_None;       // none   optional modifier that must be held for scroll wheel zooming
    float                        zoom_rate;      // 0.1f   zoom rate for scroll (e.g. 0.1f = 10% plot range every scroll click); make negative to invert
    
    std::vector<std::string>        axesNames = std::vector<std::string>(ImAxis_COUNT);
    ImPlotFlags                     _flags = ImPlotFlags_NoLegend;
    std::vector<ImPlotAxisFlags>    axesFlags = std::vector<ImPlotAxisFlags>(ImAxis_COUNT, ImPlotAxisFlags_None);
    bool                            _newColorMap = false; // to bust color cache
    bool                            _useColorMap = false;
    ImPlotColormap                  _colormap = ImPlotColormap_Deep;
    bool                            _equalAspectRatios = false;
    std::vector<ImRect>             rects = std::vector<ImRect>();
    bool                            _fitDirty = false;
    bool                            _axisfitDirty[ImAxis_COUNT] = { false, false, false, false, false, false }; 
    ImPlotInputMap                  _originalMap = ImPlotInputMap(); // custom input mapping
    bool                            localTime = false;
    bool                            iSO8601 = false;
    bool                            clock24Hour = false;
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
    inline void onChildRemoved(std::shared_ptr<mvAppItem> item) { if (item->type == mvAppItemType::mvPlotLegend){ configData.flags |= ImPlotSubplotFlags_NoLegend; configData.flags &= ~ImPlotSubplotFlags_ShareItems;}}
    inline void onChildAdd(std::shared_ptr<mvAppItem> item) { if (item->type == mvAppItemType::mvPlotLegend) { configData.flags &= ~ImPlotSubplotFlags_NoLegend; configData.flags |= ImPlotSubplotFlags_ShareItems;}}
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
};

class mvDragLine : public mvAppItem
{
public:
    mvDragLineConfig configData{};
    explicit mvDragLine(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_drag_line(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
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
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyFloatList(configData.value->data(), 2); }
    void setPyValue(PyObject* value) override;
};

class mvDragRect : public mvAppItem
{
public:
    mvDragRectConfig configData{};
    explicit mvDragRect(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_drag_rect(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
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
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData.value); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override{ return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToVectVectDouble(value); }
};

class mvInfLineSeries : public mvAppItem
{
public:
    mvInfLineSeriesConfig configData{};
    explicit mvInfLineSeries(mvUUID uuid) : mvAppItem(uuid) {}
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_positional_configuration(dict, configData); }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_inf_lines_series(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData.value); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override{ return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToVectVectDouble(value); }
};

class mvLineSeries : public mvAppItem
{
public:
    mvLineSeriesConfig configData{};
    explicit mvLineSeries(mvUUID uuid) : mvAppItem(uuid) {}
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_positional_configuration(dict, configData); }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_line_series(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData.value); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToVectVectDouble(value); }
};

class mvScatterSeries : public mvAppItem
{
public:
    mvScatterSeriesConfig configData{};
    explicit mvScatterSeries(mvUUID uuid) : mvAppItem(uuid) {}
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_positional_configuration(dict, configData); }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_scatter_series(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
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
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData.value); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToVectVectDouble(value); }
};

class mvStairSeries : public mvAppItem
{
public:
    mvStairSeriesConfig configData{};
    explicit mvStairSeries(mvUUID uuid) : mvAppItem(uuid) {}
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_positional_configuration(dict, configData); }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_stair_series(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
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
};