#pragma once

#include "mvItemRegistry.h"
#include <array>

struct mvSimplePlotConfig;
struct mvButtonConfig;
struct mvCheckboxConfig;
struct mvComboConfig;
struct mvDragFloatConfig;
struct mvDragIntConfig;
struct mvDragDoubleConfig;
struct mvDragFloatMultiConfig;
struct mvDragDoubleMultiConfig;
struct mvDragIntMultiConfig;
struct mvSliderIntConfig;
struct mvSliderFloatConfig;
struct mvSliderDoubleConfig;
struct mvSliderDoubleMultiConfig;
struct mvSliderFloatMultiConfig;
struct mvSliderIntMultiConfig;
struct mvListboxConfig;
struct mvRadioButtonConfig;
struct mvInputTextConfig;
struct mvInputIntConfig;
struct mvInputFloatConfig;
struct mvInputDoubleConfig;
struct mvInputFloatMultiConfig;
struct mvInputDoubleMultiConfig;
struct mvInputIntMultiConfig;
struct mvTextConfig;
struct mvSelectableConfig;
struct mvTabButtonConfig;
struct mvMenuItemConfig;
struct mvProgressBarConfig;
struct mvImageConfig;
struct mvImageButtonConfig;
struct mvFilterSetConfig;
struct mvTooltipConfig;
struct mvKnobFloatConfig;

namespace DearPyGui
{
    // specific part of `get_item_configuration(...)`
    void fill_configuration_dict(const mvSimplePlotConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvButtonConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvComboConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvDragFloatConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvDragFloatMultiConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvDragDoubleConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvDragDoubleMultiConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvDragIntConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvDragIntMultiConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvSliderIntConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvSliderIntMultiConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvSliderFloatConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvSliderFloatMultiConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvSliderDoubleConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvSliderDoubleMultiConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvListboxConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvRadioButtonConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvInputTextConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvInputIntConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvInputFloatConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvInputFloatMultiConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvInputDoubleConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvInputDoubleMultiConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvInputIntMultiConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvTextConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvSelectableConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvTabButtonConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvMenuItemConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvProgressBarConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvImageConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvImageButtonConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvKnobFloatConfig& inConfig, PyObject* outDict);
    void fill_configuration_dict(const mvTooltipConfig& inConfig, PyObject* outDict);

    // specific part of `configure_item(...)`
    void set_configuration(PyObject* inDict, mvSimplePlotConfig& outConfig);
    void set_configuration(PyObject* inDict, mvButtonConfig& outConfig);
    void set_configuration(PyObject* inDict, mvComboConfig& outConfig);
    void set_configuration(PyObject* inDict, mvDragFloatConfig& outConfig, mvAppItemInfo& info);
    void set_configuration(PyObject* inDict, mvDragDoubleConfig& outConfig, mvAppItemInfo& info);
    void set_configuration(PyObject* inDict, mvDragIntConfig& outConfig, mvAppItemInfo& info);
    void set_configuration(PyObject* inDict, mvDragDoubleMultiConfig& outConfig, mvAppItemInfo& info);
    void set_configuration(PyObject* inDict, mvDragFloatMultiConfig& outConfig, mvAppItemInfo& info);
    void set_configuration(PyObject* inDict, mvDragIntMultiConfig& outConfig, mvAppItemInfo& info);
    void set_configuration(PyObject* inDict, mvSliderIntConfig& outConfig, mvAppItemInfo& info);
    void set_configuration(PyObject* inDict, mvSliderIntMultiConfig& outConfig, mvAppItemInfo& info);
    void set_configuration(PyObject* inDict, mvSliderFloatConfig& outConfig, mvAppItemInfo& info);
    void set_configuration(PyObject* inDict, mvSliderFloatMultiConfig& outConfig, mvAppItemInfo& info);
    void set_configuration(PyObject* inDict, mvSliderDoubleConfig& outConfig, mvAppItemInfo& info);
    void set_configuration(PyObject* inDict, mvSliderDoubleMultiConfig& outConfig, mvAppItemInfo& info);
    void set_configuration(PyObject* inDict, mvListboxConfig& outConfig, mvAppItemInfo& info);
    void set_configuration(PyObject* inDict, mvRadioButtonConfig& outConfig);    
    void set_configuration(PyObject* inDict, mvInputTextConfig& outConfig, mvAppItemInfo& info);
    void set_configuration(PyObject* inDict, mvInputIntConfig& outConfig, mvAppItemInfo& info);
    void set_configuration(PyObject* inDict, mvInputFloatConfig& outConfig, mvAppItemInfo& info);
    void set_configuration(PyObject* inDict, mvInputFloatMultiConfig& outConfig, mvAppItemInfo& info);
    void set_configuration(PyObject* inDict, mvInputDoubleConfig& outConfig, mvAppItemInfo& info);
    void set_configuration(PyObject* inDict, mvInputDoubleMultiConfig& outConfig, mvAppItemInfo& info);
    void set_configuration(PyObject* inDict, mvInputIntMultiConfig& outConfig, mvAppItemInfo& info);
    void set_configuration(PyObject* inDict, mvTextConfig& outConfig);
    void set_configuration(PyObject* inDict, mvSelectableConfig& outConfig, mvAppItemInfo& info);
    void set_configuration(PyObject* inDict, mvTabButtonConfig& outConfig);
    void set_configuration(PyObject* inDict, mvMenuItemConfig& outConfig);
    void set_configuration(PyObject* inDict, mvProgressBarConfig& outConfig);
    void set_configuration(PyObject* inDict, mvImageConfig& outConfig);
    void set_configuration(PyObject* inDict, mvImageButtonConfig& outConfig);
    void set_configuration(PyObject* inDict, mvTooltipConfig& outConfig);
    void set_configuration(PyObject* inDict, mvKnobFloatConfig& outConfig);

    // positional args TODO: combine with above
    void set_required_configuration(PyObject* inDict, mvImageConfig& outConfig);
    void set_required_configuration(PyObject* inDict, mvImageButtonConfig& outConfig);

    // positional args TODO: combine with above
    void set_positional_configuration(PyObject* inDict, mvComboConfig& outConfig);
    void set_positional_configuration(PyObject* inDict, mvListboxConfig& outConfig);
    void set_positional_configuration(PyObject* inDict, mvRadioButtonConfig& outConfig);
    void set_positional_configuration(PyObject* inDict, mvTextConfig& outConfig);
    void set_positional_configuration(PyObject* inDict, mvTooltipConfig& outConfig, mvAppItemConfig& config);

    // data source handling
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvSimplePlotConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvComboConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvCheckboxConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvDragFloatConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvDragFloatMultiConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvDragIntConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvDragIntMultiConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvDragDoubleConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvDragDoubleMultiConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvSliderFloatConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvSliderFloatMultiConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvSliderDoubleConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvSliderDoubleMultiConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvSliderIntConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvSliderIntMultiConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvListboxConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvRadioButtonConfig& outConfig); 
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvInputTextConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvInputIntConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvInputFloatConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvInputDoubleConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvInputFloatMultiConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvInputDoubleMultiConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvInputIntMultiConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvTextConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvSelectableConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvMenuItemConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvProgressBarConfig& outConfig);
    void set_data_source(mvAppItem& item, mvUUID dataSource, mvKnobFloatConfig& outConfig);

    // draw commands
    void draw_simple_plot  (ImDrawList* drawlist, mvAppItem& item, const mvSimplePlotConfig& config);
    void draw_button       (ImDrawList* drawlist, mvAppItem& item, const mvButtonConfig& config);
    void draw_combo        (ImDrawList* drawlist, mvAppItem& item, mvComboConfig& config);
    void draw_checkbox     (ImDrawList* drawlist, mvAppItem& item, mvCheckboxConfig& config);
    void draw_drag_float   (ImDrawList* drawlist, mvAppItem& item, mvDragFloatConfig& config);
    void draw_drag_floatx  (ImDrawList* drawlist, mvAppItem& item, mvDragFloatMultiConfig& config);
    void draw_drag_double  (ImDrawList* drawlist, mvAppItem& item, mvDragDoubleConfig& config);
    void draw_drag_doublex (ImDrawList* drawlist, mvAppItem& item, mvDragDoubleMultiConfig& config);
    void draw_drag_int     (ImDrawList* drawlist, mvAppItem& item, mvDragIntConfig& config);
    void draw_drag_intx    (ImDrawList* drawlist, mvAppItem& item, mvDragIntMultiConfig& config);
    void draw_slider_float (ImDrawList* drawlist, mvAppItem& item, mvSliderFloatConfig& config);
    void draw_slider_floatx(ImDrawList* drawlist, mvAppItem& item, mvSliderFloatMultiConfig& config);
    void draw_slider_double(ImDrawList* drawlist, mvAppItem& item, mvSliderDoubleConfig& config);
    void draw_slider_doublex(ImDrawList* drawlist, mvAppItem& item, mvSliderDoubleMultiConfig& config);
    void draw_slider_int   (ImDrawList* drawlist, mvAppItem& item, mvSliderIntConfig& config);
    void draw_slider_intx  (ImDrawList* drawlist, mvAppItem& item, mvSliderIntMultiConfig& config);
    void draw_listbox      (ImDrawList* drawlist, mvAppItem& item, mvListboxConfig& config);
    void draw_radio_button (ImDrawList* drawlist, mvAppItem& item, mvRadioButtonConfig& config);
    void draw_input_text   (ImDrawList* drawlist, mvAppItem& item, mvInputTextConfig& config);
    void draw_input_float  (ImDrawList* drawlist, mvAppItem& item, mvInputFloatConfig& config);
    void draw_input_floatx (ImDrawList* drawlist, mvAppItem& item, mvInputFloatMultiConfig& config);
    void draw_input_double (ImDrawList* drawlist, mvAppItem& item, mvInputDoubleConfig& config);
    void draw_input_doublex(ImDrawList* drawlist, mvAppItem& item, mvInputDoubleMultiConfig& config);
    void draw_input_int    (ImDrawList* drawlist, mvAppItem& item, mvInputIntConfig& config);
    void draw_input_intx   (ImDrawList* drawlist, mvAppItem& item, mvInputIntMultiConfig& config);
    void draw_text         (ImDrawList* drawlist, mvAppItem& item, mvTextConfig& config);
    void draw_selectable   (ImDrawList* drawlist, mvAppItem& item, mvSelectableConfig& config);
    void draw_tab_button   (ImDrawList* drawlist, mvAppItem& item, mvTabButtonConfig& config);
    void draw_menu_item    (ImDrawList* drawlist, mvAppItem& item, mvMenuItemConfig& config);
    void draw_progress_bar (ImDrawList* drawlist, mvAppItem& item, mvProgressBarConfig& config);
    void draw_image        (ImDrawList* drawlist, mvAppItem& item, mvImageConfig& config);
    void draw_image_button (ImDrawList* drawlist, mvAppItem& item, mvImageButtonConfig& config);
    void draw_filter_set   (ImDrawList* drawlist, mvAppItem& item, mvFilterSetConfig& config);
    void draw_knob_float   (ImDrawList* drawlist, mvAppItem& item, mvKnobFloatConfig& config);
    void draw_separator    (ImDrawList* drawlist, mvAppItem& item);
    void draw_spacer       (ImDrawList* drawlist, mvAppItem& item);
    void draw_menubar      (ImDrawList* drawlist, mvAppItem& item);
    void draw_viewport_menubar(ImDrawList* drawlist, mvAppItem& item);
    void draw_clipper      (ImDrawList* drawlist, mvAppItem& item);    
    void draw_tooltip      (ImDrawList* drawlist, mvAppItem& item);      
}

enum class mvComboHeightMode
{
    mvComboHeight_Small = 0L,
    mvComboHeight_Regular,
    mvComboHeight_Large,
    mvComboHeight_Largest
};

struct mvSimplePlotConfig
{
    std::shared_ptr<std::vector<float>> value = std::make_shared<std::vector<float>>(std::vector<float>{0.0f});
    std::string               overlay;
    float                     scaleMin = 0.0f;
    float                     scaleMax = 0.0f;
    bool                      histogram = false;
    bool                      autosize = true;
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
    std::shared_ptr<std::string>       value = std::make_shared<std::string>("");
    std::string              disabled_value;
};

struct mvCheckboxConfig
{
    std::shared_ptr<bool> value = std::make_shared<bool>(false);
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
    std::shared_ptr<float>        value = std::make_shared<float>(0.0f);
    float               disabled_value = 0.0f;
};

struct mvDragDoubleConfig
{
    float               speed = 1.0f;
    double              minv = 0.0;
    double              maxv = 100.0;
    std::string         format = "%.3f";
    ImGuiInputTextFlags flags = ImGuiSliderFlags_None;
    ImGuiInputTextFlags stor_flags = ImGuiSliderFlags_None;
    std::shared_ptr<double>       value = std::make_shared<double>(0.0);
    double              disabled_value = 0.0;
};

struct mvDragIntConfig
{
    float               speed = 1.0f;
    int                 minv = 0;
    int                 maxv = 100;
    std::string         format = "%d";
    ImGuiInputTextFlags flags = ImGuiSliderFlags_None;
    ImGuiInputTextFlags stor_flags = ImGuiSliderFlags_None;
    std::shared_ptr<int>          value = std::make_shared<int>(0);
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
    std::shared_ptr<std::array<int, 4>> value = std::make_shared<std::array<int, 4>>(std::array<int, 4>{0, 0, 0, 0});
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
    std::shared_ptr<std::array<float, 4>> value = std::make_shared<std::array<float, 4>>(std::array<float, 4>{0.0f, 0.0f, 0.0f, 0.0f});
    float                       disabled_value[4]{};  
};

struct mvDragDoubleMultiConfig
{
    float                       speed = 1.0f;
    double                      minv = 0.0;
    double                      maxv = 100.0;
    std::string                 format = "%.3f";
    ImGuiInputTextFlags         flags = ImGuiSliderFlags_None;
    ImGuiInputTextFlags         stor_flags = ImGuiSliderFlags_None;
    int                         size = 4;
    std::shared_ptr<std::array<double, 4>>value = std::make_shared<std::array<double, 4>>(std::array<double, 4>{0.0, 0.0, 0.0, 0.0});
    double                      disabled_value[4]{};
};

struct mvSliderIntConfig
{
    int                 minv = 0;
    int                 maxv = 100;
    std::string         format = "%d";
    bool                vertical = false;
    ImGuiInputTextFlags flags = ImGuiSliderFlags_None;
    ImGuiInputTextFlags stor_flags = ImGuiSliderFlags_None;
    std::shared_ptr<int>          value = std::make_shared<int>(0);
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
    std::shared_ptr<float>        value = std::make_shared<float>(0.0f);
    float               disabled_value = 0.0f;
};

struct mvSliderDoubleConfig
{
    double               minv = 0.0;
    double               maxv = 100.0;
    std::string          format = "%.3f";
    bool                 vertical = false;
    ImGuiInputTextFlags  flags = ImGuiSliderFlags_None;
    ImGuiInputTextFlags  stor_flags = ImGuiSliderFlags_None;
    std::shared_ptr<double>        value = std::make_shared<double>(0.0);
    double               disabled_value = 0.0;
};

struct mvSliderFloatMultiConfig
{
    float                       minv = 0.0f;
    float                       maxv = 100.0f;
    std::string                 format = "%.3f";
    ImGuiInputTextFlags         flags = ImGuiSliderFlags_None;
    ImGuiInputTextFlags         stor_flags = ImGuiSliderFlags_None;
    int                         size = 4;
    std::shared_ptr<std::array<float, 4>> value = std::make_shared<std::array<float, 4>>(std::array<float, 4>{0.0f, 0.0f, 0.0f, 0.0f});
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
    std::shared_ptr<std::array<int, 4>> value = std::make_shared<std::array<int, 4>>(std::array<int, 4>{0, 0, 0, 0});
    int                       disabled_value[4]{};
};

struct mvSliderDoubleMultiConfig
{
    double                       minv = 0.0;
    double                       maxv = 100.0;
    std::string                  format = "%d";
    ImGuiInputTextFlags          flags = ImGuiSliderFlags_None;
    ImGuiInputTextFlags          stor_flags = ImGuiSliderFlags_None;
    int                          size = 4;
    std::shared_ptr<std::array<double, 4>> value = std::make_shared<std::array<double, 4>>(std::array<double, 4>{0.0, 0.0, 0.0, 0.0});
    double                       disabled_value[4]{};
};

struct mvListboxConfig
{
    std::vector<std::string> names;
    int                      itemsHeight = 3; // number of items to show (default -1)
    std::vector<const char*> charNames;
    int                      index = 0;
    int                      disabledindex = 0;
    std::shared_ptr<std::string>       value = std::make_shared<std::string>("");
    std::string              disabled_value;
};

struct mvRadioButtonConfig
{
    std::vector<std::string> itemnames;
    bool                     horizontal = false;
    int                      index = 0;
    int                      disabledindex = 0;
    std::shared_ptr<std::string>       value = std::make_shared<std::string>("");
    std::string              disabled_value = "";
};

struct mvInputTextConfig
{
    std::string         hint;
    bool                multiline = false;
    ImGuiInputTextFlags flags = 0;
    ImGuiInputTextFlags stor_flags = 0;
    std::shared_ptr<std::string>  value = std::make_shared<std::string>("");
    std::string         disabled_value = "";
};

struct mvInputIntConfig
{
    int                 step = 1;
    int                 step_fast = 100;
    int                 minv = 0;
    int                 maxv = 100;
    bool                min_clamped = false;
    bool                max_clamped = false;
    ImGuiInputTextFlags flags = 0;
    ImGuiInputTextFlags stor_flags = 0;
    int                 last_value = 0;
    std::shared_ptr<int>          value = std::make_shared<int>(0);
    int                 disabled_value = 0;
};

struct mvInputFloatConfig
{
    float               minv = 0.0f;
    float               maxv = 100.0f;
    bool                min_clamped = false;
    bool                max_clamped = false;
    std::string         format = "%.3f";
    float               step = 0.1f;
    float               step_fast = 1.0f;
    ImGuiInputTextFlags flags = 0;
    ImGuiInputTextFlags stor_flags = 0;
    float               last_value = 0.0f;
    std::shared_ptr<float>        value = std::make_shared<float>(0.0f);
    float               disabled_value = 0.0f;
};

struct mvInputDoubleConfig
{
    double              minv = 0.0;
    double              maxv = 100.0;
    bool                min_clamped = false;
    bool                max_clamped = false;
    std::string         format = "%.3f";
    double              step = 0.1;
    double              step_fast = 1.0;
    ImGuiInputTextFlags flags = 0;
    ImGuiInputTextFlags stor_flags = 0;
    double              last_value = 0.0;
    std::shared_ptr<double>        value = std::make_shared<double>(0.0);
    double              disabled_value = 0.0;
};

struct mvInputFloatMultiConfig
{
    float                       minv = 0.0f;
    float                       maxv = 100.0f;
    bool                        min_clamped = false;
    bool                        max_clamped = false;
    std::string                 format = "%.3f";
    ImGuiInputTextFlags         flags = 0;
    ImGuiInputTextFlags         stor_flags = 0;
    std::array<float, 4>        last_value = { 0.0f, 0.0f, 0.0f, 0.0f };
    int                         size = 4;
    std::shared_ptr<std::array<float, 4>> value = std::make_shared<std::array<float, 4>>(std::array<float, 4>{0.0f, 0.0f, 0.0f, 0.0f});
    float                       disabled_value[4]{};
};

struct mvInputIntMultiConfig
{
    int                       minv = 0;
    int                       maxv = 100;
    bool                      min_clamped = false;
    bool                      max_clamped = false;
    ImGuiInputTextFlags       flags = 0;
    ImGuiInputTextFlags       stor_flags = 0;
    std::array<int, 4>        last_value = { 0, 0, 0, 0 };
    int                       size = 4;
    std::shared_ptr<std::array<int, 4>> value = std::make_shared<std::array<int, 4>>(std::array<int, 4>{0, 0, 0, 0});
    int                       disabled_value[4]{};
};

struct mvInputDoubleMultiConfig
{
    double                       minv = 0.0;
    double                       maxv = 100.0;
    bool                         min_clamped = false;
    bool                         max_clamped = false;
    std::string                  format = "%.3f";
    ImGuiInputTextFlags          flags = 0;
    ImGuiInputTextFlags          stor_flags = 0;
    std::array<double, 4>        last_value = { 0.0f, 0.0f, 0.0f, 0.0f };
    int                          size = 4;
    std::shared_ptr<std::array<double, 4>> value = std::make_shared<std::array<double, 4>>(std::array<double, 4>{0.0, 0.0, 0.0, 0.0});
    double                       disabled_value[4]{};
};

struct mvTextConfig
{
    mvColor            color = { -1.0f, 0.0f, 0.0f, 1.0f };
    int                wrap = -1;
    bool               bullet = false;
    bool               show_label = false;
    std::shared_ptr<std::string> value = std::make_shared<std::string>("");
    std::string        disabled_value = "";
};

struct mvSelectableConfig
{
    ImGuiSelectableFlags flags = ImGuiSelectableFlags_None;
    std::shared_ptr<bool>          value = std::make_shared<bool>(false);
    bool                 disabled_value = false;
};

struct mvTabButtonConfig
{
    ImGuiTabItemFlags flags = ImGuiTabItemFlags_None;
};

struct mvMenuItemConfig
{
    std::string shortcut;
    bool        check = false;
    std::shared_ptr<bool> value = std::make_shared<bool>(false);
    bool        disabled_value = false;
};

struct mvProgressBarConfig
{
    std::string  overlay;
    std::shared_ptr<float> value = std::make_shared<float>(0.0f);
    float        disabled_value = 0.0f;
};

struct mvImageConfig
{
    // pointer to existing item or internal
    std::shared_ptr<mvAppItem> texture = nullptr;
    mvUUID                     textureUUID = 0;
    mvVec2                     uv_min = { 0.0f, 0.0f };
    mvVec2                     uv_max = { 1.0f, 1.0f };
    mvColor                    tintColor = { 1.0f, 1.0f, 1.0f, 1.0f };
    mvColor                    borderColor = { 0.0f, 0.0f, 0.0f, 0.0f };
    bool                       _internalTexture = false; // create a local texture if necessary
};

struct mvImageButtonConfig
{
    // pointer to existing item or internal
    std::shared_ptr<mvAppItem> texture = nullptr;
    mvUUID                     textureUUID = 0;
    mvVec2	                   uv_min = { 0.0f, 0.0f };
    mvVec2	                   uv_max = { 1.0f, 1.0f };
    mvColor                    tintColor = { 1.0f, 1.0f, 1.0f, 1.0f };
    mvColor                    backgroundColor = { 0.0f, 0.0f, 0.0f, 0.0f };
    int                        framePadding = -1;
    bool                       _internalTexture = false; // create a local texture if necessary
};

struct mvFilterSetConfig
{
    ImGuiTextFilter imguiFilter;
};

struct mvTooltipConfig
{
    float        activation_delay = 0.5;
    bool         hide_on_move = true;
};

struct mvKnobFloatConfig
{
    std::shared_ptr<float> value = std::make_shared<float>(0.0f);
    float        disabled_value = 0.0f;
    float        minv = 0.0f;
    float        maxv = 100.0f;
    float        step = 50.0f;
};

//-----------------------------------------------------------------------------
// Old Classes, in the process of removing OOP crap
//-----------------------------------------------------------------------------

class mvSimplePlot : public mvAppItem
{
public:
    mvSimplePlotConfig configData{};
    explicit mvSimplePlot(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_simple_plot(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyList(*configData.value); }
    void setPyValue(PyObject* value) override;
};

class mvButton : public mvAppItem
{
public:
    mvButtonConfig configData{};
    explicit mvButton(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_button(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
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
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override{ return ToPyFloat(*configData.value); }
    void setPyValue(PyObject* value) override{ *configData.value = ToFloat(value); }
};

class mvDragDouble : public mvAppItem
{
public:
    mvDragDoubleConfig configData{};
    explicit mvDragDouble(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_drag_double(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData, info); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyDouble(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToDouble(value); }
};

class mvDragInt : public mvAppItem
{

public:
    mvDragIntConfig configData{};
    explicit mvDragInt(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_drag_int(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData, info); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
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
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyFloatList(configData.value->data(), 4); }
    void setPyValue(PyObject* value) override;
};

class mvDragDoubleMulti : public mvAppItem
{
public:
    mvDragDoubleMultiConfig configData{};
    explicit mvDragDoubleMulti(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_drag_doublex(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData, info); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
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
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyFloat(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToFloat(value); }
};

class mvSliderDouble : public mvAppItem
{
public:
    mvSliderDoubleConfig configData{};
    explicit mvSliderDouble(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_slider_double(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData, info); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyDouble(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToDouble(value); }
};

class mvSliderFloatMulti : public mvAppItem
{
public:
    mvSliderFloatMultiConfig configData{};
    explicit mvSliderFloatMulti(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_slider_floatx(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData, info); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyFloatList(configData.value->data(), 4); }
    void setPyValue(PyObject* value) override;
};

class mvSliderDoubleMulti : public mvAppItem
{
public:
    mvSliderDoubleMultiConfig configData{};
    explicit mvSliderDoubleMulti(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_slider_doublex(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData, info); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
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
    void setPyValue(PyObject* value) override;
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override {return ToPyString(*configData.value);}
};

class mvRadioButton : public mvAppItem
{
public:
    mvRadioButtonConfig configData{};
    explicit mvRadioButton(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_radio_button(drawlist, *this, configData); }
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_positional_configuration(dict, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void setPyValue(PyObject* value) override;
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyString(*configData.value); }
};

class mvInputText : public mvAppItem
{

public:
    mvInputTextConfig configData{};
    explicit mvInputText(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_input_text(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData, info); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override{ return ToPyString(*configData.value); }
    void setPyValue(PyObject* value) override{ *configData.value = ToString(value); }
};

class mvInputInt : public mvAppItem
{
public:
    mvInputIntConfig configData{};
    explicit mvInputInt(mvUUID uuid) : mvAppItem(uuid) { configData.last_value = *configData.value; }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_input_int(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData, info); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override{ return ToPyInt(*configData.value); }
    void setPyValue(PyObject* value) override{ *configData.value = ToInt(value); configData.last_value = *configData.value;}
};

class mvInputIntMulti : public mvAppItem
{
public:
    mvInputIntMultiConfig configData{};
    explicit mvInputIntMulti(mvUUID uuid) : mvAppItem(uuid){ configData.last_value = *configData.value; }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_input_intx(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData, info); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override{ return ToPyIntList(configData.value->data(), 4); }
    void setPyValue(PyObject* value) override;
};

class mvInputFloat : public mvAppItem
{
public:
    mvInputFloatConfig configData{};
    explicit mvInputFloat(mvUUID uuid) : mvAppItem(uuid) { configData.last_value = *configData.value; }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_input_float(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData, info); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyFloat(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToFloat(value); configData.last_value = *configData.value; }
};

class mvInputFloatMulti : public mvAppItem
{
public:
    mvInputFloatMultiConfig configData{};
    explicit mvInputFloatMulti(mvUUID uuid) : mvAppItem(uuid) { configData.last_value = *configData.value; }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_input_floatx(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData, info); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyFloatList(configData.value->data(), 4); }
    void setPyValue(PyObject* value) override;
};

class mvInputDouble : public mvAppItem
{
public:
    mvInputDoubleConfig configData{};
    explicit mvInputDouble(mvUUID uuid) : mvAppItem(uuid) { configData.last_value = *configData.value; }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_input_double(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData, info); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyDouble(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToDouble(value); configData.last_value = *configData.value; }
};

class mvInputDoubleMulti : public mvAppItem
{
public:
    mvInputDoubleMultiConfig configData{};
    explicit mvInputDoubleMulti(mvUUID uuid) : mvAppItem(uuid) { configData.last_value = *configData.value; }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_input_doublex(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData, info); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyFloatList(configData.value->data(), 4); }
    void setPyValue(PyObject* value) override;
};

class mvText : public mvAppItem
{

public:
    mvTextConfig configData{};
    explicit mvText(mvUUID uuid) : mvAppItem(uuid) { *configData.value = "Not Specified"; }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_text(drawlist, *this, configData); }
    void handleSpecificPositionalArgs(PyObject* dict) override { DearPyGui::set_positional_configuration(dict, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyString(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToString(value); }
};

class mvSelectable : public mvAppItem
{
public:
    mvSelectableConfig configData{};
    explicit mvSelectable(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_selectable(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData, info); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyBool(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToBool(value); }
};

class mvTabButton : public mvAppItem
{
public:
    mvTabButtonConfig configData{};
    explicit mvTabButton(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_tab_button(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
};

class mvMenuItem : public mvAppItem
{

public:
    mvMenuItemConfig configData{};
    explicit mvMenuItem(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_menu_item(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyBool(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToBool(value); }
};

class mvProgressBar : public mvAppItem
{
public:
    mvProgressBarConfig configData{};
    explicit mvProgressBar(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_progress_bar(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyFloat(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToFloat(value); }
};

class mvImage : public mvAppItem
{
public:
    mvImageConfig configData{};
    mvImage(mvUUID uuid) : mvAppItem(uuid){ config.width = 0; config.height = 0;}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_image(drawlist, *this, configData); }
    void handleSpecificRequiredArgs(PyObject* dict) override { DearPyGui::set_required_configuration(dict, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
};

class mvImageButton : public mvAppItem
{
public:
    mvImageButtonConfig configData{};
    mvImageButton(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_image_button(drawlist, *this, configData); }
    void handleSpecificRequiredArgs(PyObject* dict) override { DearPyGui::set_required_configuration(dict, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
};

class mvKnobFloat : public mvAppItem
{

public:
    mvKnobFloatConfig configData{};
    explicit mvKnobFloat(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_knob_float(drawlist, *this, configData); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }
    void setDataSource(mvUUID dataSource) override { DearPyGui::set_data_source(*this, dataSource, configData); }
    void* getValue() override { return &configData.value; }
    PyObject* getPyValue() override { return ToPyFloat(*configData.value); }
    void setPyValue(PyObject* value) override { *configData.value = ToFloat(value); }
};

class mvFilterSet : public mvAppItem
{
public:
    mvFilterSetConfig configData{};
    explicit mvFilterSet(mvUUID uuid) : mvAppItem(uuid) {}
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_filter_set(drawlist, *this, configData); }
    void setPyValue(PyObject* value) override;
    PyObject* getPyValue() override { return ToPyString(std::string(configData.imguiFilter.InputBuf)); }
};

class mvTooltip : public mvAppItem
{
public:
    mvTooltipConfig configData{};
    explicit mvTooltip(mvUUID uuid) : mvAppItem(uuid) { config.show = true; } //has to be shown to check hovering
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_tooltip(drawlist, *this); }
    void handleSpecificRequiredArgs(PyObject* dict) override { DearPyGui::set_positional_configuration(dict, configData, config); }
    void handleSpecificKeywordArgs(PyObject* dict) override { DearPyGui::set_configuration(dict, configData); }
    void getSpecificConfiguration(PyObject* dict) override { DearPyGui::fill_configuration_dict(configData, dict); }

// TODO: make these members private
    double change_time = 0;
    bool hovered_last_frame = false;
    ImVec2 last_mouse_pos;
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

class mvViewportMenuBar : public mvAppItem
{
public:
    explicit mvViewportMenuBar(mvUUID uuid) : mvAppItem(uuid)
    {

        // TODO use code below to set item height when font and scale systems are done
        //float FontSize = ImGui::GetFontSize(); // = Base Font Size * Current Window Scale
        //ImGuiStyle currentStyle = ImGui::GetStyle(); // = Padding for the Top and Bottom
        //_height = int(currentStyle.FramePadding.y * 2 + FontSize);
        config.height = 21;
    }
    void draw(ImDrawList* drawlist, float x, float y) override { DearPyGui::draw_viewport_menubar(drawlist, *this); }
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
