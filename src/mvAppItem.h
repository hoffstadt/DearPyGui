#pragma once

//-----------------------------------------------------------------------------
// mvAppItem
//
//     - mvAppItem is the abstract base class for all DearPygui widgets.
//     
//-----------------------------------------------------------------------------

#include <string>
#include <vector>
#include <map>
#include <imgui.h>
#include "mvAppItemState.h"
#include "mvCallbackRegistry.h"
#include "mvPyUtils.h"
#include <implot_internal.h>
#include "mvAppItemTypes.inc"

// forward declarations
class mvThemeManager;
class mvAppItem;
class mvItemHandlerRegistry;
class mvTheme;
class mvThemeComponent;

//-----------------------------------------------------------------------------
// mvAppItemType
//-----------------------------------------------------------------------------
enum class mvAppItemType
{
    None = -1, All, 
    #define X(el) el,
    MV_ITEM_TYPES
    #undef X
    ItemTypeCount
};

enum class StorageValueTypes
{
    None = 0,
    Int, Int4, 
    Float, Float4, FloatVect, 
    Double, Double4, DoubleVect,
    Series, Bool, String, UUID,
    Time, Color, Texture
};

enum ItemDescriptionFlags
{
    MV_ITEM_DESC_DEFAULT     = 0,
    MV_ITEM_DESC_ROOT        = 1 << 1,
    MV_ITEM_DESC_CONTAINER   = 1 << 2,
    MV_ITEM_DESC_HANDLER     = 1 << 3, // todo: rename descriptively
    MV_ITEM_DESC_DRAW_CMP    = 1 << 4  // has draw component
};

enum class mvLibType
{
    MV_IMGUI = 0,
    OT = 1,
    MV_IMNODES = 2
};

struct ScopedID
{
    ScopedID(void* id) { ImGui::PushID(id); }
    ScopedID(mvUUID id) { ImGui::PushID((int)id); }
    ~ScopedID() { ImGui::PopID(); }
};

namespace DearPyGui
{
    std::shared_ptr<mvAppItem>                                CreateEntity                    (mvAppItemType type, mvUUID id);
    int                                             GetEntityDesciptionFlags        (mvAppItemType type);
    int                                             GetEntityTargetSlot             (mvAppItemType type);
    StorageValueTypes                               GetEntityValueType              (mvAppItemType type);
    const char*                                     GetEntityTypeString             (mvAppItemType type);
    int                                             GetApplicableState              (mvAppItemType type);
    const std::vector<std::pair<std::string, i32>>& GetAllowableParents             (mvAppItemType type);
    const std::vector<std::pair<std::string, i32>>& GetAllowableChildren            (mvAppItemType type);
    std::shared_ptr<mvThemeComponent>&                        GetClassThemeComponent          (mvAppItemType type);
    std::shared_ptr<mvThemeComponent>&                        GetDisabledClassThemeComponent  (mvAppItemType type);
    mvPythonParser                                  GetEntityParser                 (mvAppItemType type);
    void                                            OnChildAdded                    (mvAppItem* item, std::shared_ptr<mvAppItem> child);
    void                                            OnChildRemoved                  (mvAppItem* item, std::shared_ptr<mvAppItem> child);
}

struct mvAppItemInfo
{
    std::string internalLabel; // label passed into imgui
    mvAppItem*  parentPtr = nullptr;
    int         location = -1;
    bool        showDebug = false;
        
    // next frame triggers
    bool focusNextFrame           = false;
    bool triggerAlternativeAction = false;
    bool shownLastFrame           = false;
    bool hiddenLastFrame          = false;
    bool enabledLastFrame         = false;
    bool disabledLastFrame        = false;

    // previous frame cache
    ImVec2 previousCursorPos = { 0.0f, 0.0f };

    // dirty flags
    bool dirty_size = true;
    bool dirtyPos   = false;
};

struct mvAppItemConfig
{
    mvUUID      source = 0;
    mvUUID      parent = 0;
    std::string specifiedLabel;
    std::string filter;
    std::string alias;
    std::string payloadType = "$$DPG_PAYLOAD";
    int         width = 0;
    int         height = 0;
    float       indent = -1.0f;
    float       trackOffset = 0.5f; // 0.0f:top, 0.5f:center, 1.0f:bottom
    bool        show             = true;
    bool        enabled          = true;
    bool        searchLast       = false;
    bool        searchDelayed    = false;
    bool        useInternalLabel = true; // when false, will use specificed label
    bool        tracked          = false;
    PyObject*   callback         = nullptr;
    PyObject*   user_data        = nullptr;
    PyObject*   dragCallback     = nullptr;
    PyObject*   dropCallback     = nullptr;
};

struct mvAppItemDrawInfo
{
    mvMat4 transform         = mvIdentityMat4();
    mvMat4 appliedTransform  = mvIdentityMat4(); // only used by nodes
    long   cullMode          = 0; // mvCullMode_None
    bool   perspectiveDivide = false;
    bool   depthClipping     = false;
    float  clipViewport[6]   = { 0.0f, 0.0f, 1.0f, 1.0f, -1.0f, 1.0f }; // top leftx, top lefty, width, height, min depth, maxdepth
};

//-----------------------------------------------------------------------------
// mvAppItem
//-----------------------------------------------------------------------------
class mvAppItem
{

public:

    mvAppItemType                type = mvAppItemType::None;
    mvUUID                       uuid = 0;
    mvAppItemState               state;
    mvAppItemInfo                info{};
    mvAppItemConfig              config{};
    std::shared_ptr<mvItemHandlerRegistry> handlerRegistry = nullptr;
    std::shared_ptr<mvAppItem>             font = nullptr;
    std::shared_ptr<mvTheme>               theme = nullptr;
    std::shared_ptr<mvAppItemDrawInfo>     drawInfo = nullptr;

    // slots
    //   * 0 : mvFileExtension, mvFontRangeHint, mvNodeLink, mvAnnotation
    //         mvDragLine, mvDragPoint, mvDragRect, mvLegend, mvTableColumn
    //   * 1 : Most widgets
    //   * 2 : Draw Commands
    //   * 3 : mvDragPayload
    std::vector<std::shared_ptr<mvAppItem>> childslots[4] = { {}, {}, {}, {} };

public:

    mvAppItem(mvUUID uuid);
    mvAppItem(const mvAppItem& other) = delete; // copy constructor
    mvAppItem(mvAppItem&& other)      = delete; // move constructor

    virtual ~mvAppItem();

    // actual immediate mode drawing instructions
    virtual void draw(ImDrawList* drawlist, f32 x, f32 y) = 0;

    //-----------------------------------------------------------------------------
    // usually used for iterating through items and performing an action
    // other than drawing.
    //-----------------------------------------------------------------------------
    virtual void customAction(void* data = nullptr) {};

    //-----------------------------------------------------------------------------
    // These methods handle setting the widget's value using PyObject*'s or
    // returning the actual value. These are mostly overridden by the
    // mvTypeBase classes
    //-----------------------------------------------------------------------------
    virtual void*     getValue() { return nullptr; }
    virtual PyObject* getPyValue() { return GetPyNone(); }
    virtual void      setPyValue(PyObject* value) { }

    // used to check arguments, get/set configurations
    void handleKeywordArgs(PyObject* dict, const std::string& parser);  // python dictionary acts as an out parameter 

    // used by derived items to register their arguments
    virtual void handleSpecificRequiredArgs  (PyObject* args)  {}
    virtual void handleSpecificPositionalArgs(PyObject* args)  {}
    virtual void handleSpecificKeywordArgs   (PyObject* dict)  {} // called by handleKeywordArgs
    virtual void getSpecificConfiguration    (PyObject* dict)  {}

    //-----------------------------------------------------------------------------
    // callbacks
    //-----------------------------------------------------------------------------
    [[nodiscard]] PyObject* getCallback(b8 ignore_enabled = true);  // returns the callback. If ignore_enable false and item is disabled then no callback will be returned.
       
    //-----------------------------------------------------------------------------
    // config setters
    //-----------------------------------------------------------------------------
    virtual void setDataSource(mvUUID value);
       
};

inline bool mvClipPoint(float clipViewport[6], mvVec4& point)
{

    if (point.x < clipViewport[0]) return true;
    if (point.x > clipViewport[0] + clipViewport[2]) return true;

    if (point.y > clipViewport[1]) return true;
    if (point.y < clipViewport[1] - clipViewport[3]) return true;

    if (point.z < clipViewport[4]) return true;
    if (point.z > clipViewport[5]) return true;

    return false;
}

inline constexpr const char*
GetEntityCommand(mvAppItemType type)
{
    switch (type)
    {

    case mvAppItemType::mvButton:                      return "add_button";
    case mvAppItemType::mvCheckbox:                    return "add_checkbox";
    case mvAppItemType::mvInputText:                   return "add_input_text";
    case mvAppItemType::mvRadioButton:                 return "add_radio_button";
    case mvAppItemType::mvTabBar:                      return "add_tab_bar";
    case mvAppItemType::mvTab:                         return "add_tab";
    case mvAppItemType::mvImage:                       return "add_image";
    case mvAppItemType::mvMenuBar:                     return "add_menu_bar";
    case mvAppItemType::mvViewportMenuBar:             return "add_viewport_menu_bar";
    case mvAppItemType::mvMenu:                        return "add_menu";
    case mvAppItemType::mvMenuItem:                    return "add_menu_item";
    case mvAppItemType::mvChildWindow:                 return "add_child_window";
    case mvAppItemType::mvGroup:                       return "add_group";
    case mvAppItemType::mvSliderFloat:                 return "add_slider_float";
    case mvAppItemType::mvSliderDouble:                return "add_slider_double";
    case mvAppItemType::mvSliderInt:                   return "add_slider_int";
    case mvAppItemType::mvFilterSet:                   return "add_filter_set";
    case mvAppItemType::mvDragFloat:                   return "add_drag_float";
    case mvAppItemType::mvDragDouble:                  return "add_drag_double";
    case mvAppItemType::mvDragInt:                     return "add_drag_int";
    case mvAppItemType::mvInputFloat:                  return "add_input_float";
    case mvAppItemType::mvInputDouble:                 return "add_input_double";
    case mvAppItemType::mvInputInt:                    return "add_input_int";
    case mvAppItemType::mvColorEdit:                   return "add_color_edit";
    case mvAppItemType::mvClipper:                     return "add_clipper";
    case mvAppItemType::mvColorPicker:                 return "add_color_picker";
    case mvAppItemType::mvTooltip:                     return "add_tooltip";
    case mvAppItemType::mvCollapsingHeader:            return "add_collapsing_header";
    case mvAppItemType::mvSeparator:                   return "add_separator";
    case mvAppItemType::mvListbox:                     return "add_listbox";
    case mvAppItemType::mvText:                        return "add_text";
    case mvAppItemType::mvCombo:                       return "add_combo";
    case mvAppItemType::mvPlot:                        return "add_plot";
    case mvAppItemType::mvSimplePlot:                  return "add_simple_plot";
    case mvAppItemType::mvDrawlist:                    return "add_drawlist";
    case mvAppItemType::mvWindowAppItem:               return "add_window";
    case mvAppItemType::mvSelectable:                  return "add_selectable";
    case mvAppItemType::mvTreeNode:                    return "add_tree_node";
    case mvAppItemType::mvProgressBar:                 return "add_progress_bar";
    case mvAppItemType::mvSpacer:                      return "add_spacer";
    case mvAppItemType::mvImageButton:                 return "add_image_button";
    case mvAppItemType::mvTimePicker:                  return "add_time_picker";
    case mvAppItemType::mvDatePicker:                  return "add_date_picker";
    case mvAppItemType::mvColorButton:                 return "add_color_button";
    case mvAppItemType::mvFileDialog:                  return "add_file_dialog";
    case mvAppItemType::mvTabButton:                   return "add_tab_button";
    case mvAppItemType::mvDrawNode:                    return "add_draw_node";
    case mvAppItemType::mvNodeEditor:                  return "add_node_editor";
    case mvAppItemType::mvNode:                        return "add_node";
    case mvAppItemType::mvNodeAttribute:               return "add_node_attribute";
    case mvAppItemType::mvTable:                       return "add_table";
    case mvAppItemType::mvTableColumn:                 return "add_table_column";
    case mvAppItemType::mvTableRow:                    return "add_table_row";
    case mvAppItemType::mvDrawLine:                    return "draw_line";
    case mvAppItemType::mvDrawArrow:                   return "draw_arrow";
    case mvAppItemType::mvDrawTriangle:                return "draw_triangle";
    case mvAppItemType::mvDrawImageQuad:               return "draw_image_quad";
    case mvAppItemType::mvDrawCircle:                  return "draw_circle";
    case mvAppItemType::mvDrawEllipse:                 return "draw_ellipse";
    case mvAppItemType::mvDrawBezierCubic:             return "draw_bezier_cubic";
    case mvAppItemType::mvDrawBezierQuadratic:         return "draw_bezier_quadratic";
    case mvAppItemType::mvDrawQuad:                    return "draw_quad";
    case mvAppItemType::mvDrawRect:                    return "draw_rectangle";
    case mvAppItemType::mvDrawText:                    return "draw_text";
    case mvAppItemType::mvDrawPolygon:                 return "draw_polygon";
    case mvAppItemType::mvDrawPolyline:                return "draw_polyline";
    case mvAppItemType::mvDrawImage:                   return "draw_image";
    case mvAppItemType::mvDragFloatMulti:              return "add_drag_floatx";
    case mvAppItemType::mvDragDoubleMulti:             return "add_drag_doublex";
    case mvAppItemType::mvDragIntMulti:                return "add_drag_intx";
    case mvAppItemType::mvSliderFloatMulti:            return "add_slider_floatx";
    case mvAppItemType::mvSliderDoubleMulti:           return "add_slider_doublex";
    case mvAppItemType::mvSliderIntMulti:              return "add_slider_intx";
    case mvAppItemType::mvInputIntMulti:               return "add_input_intx";
    case mvAppItemType::mvInputFloatMulti:             return "add_input_floatx";
    case mvAppItemType::mvInputDoubleMulti:            return "add_input_doublex";
    case mvAppItemType::mvDragPoint:                   return "add_drag_point";
    case mvAppItemType::mvDragRect:                    return "add_drag_rect";
    case mvAppItemType::mvDragLine:                    return "add_drag_line";
    case mvAppItemType::mvAnnotation:                  return "add_plot_annotation";
    case mvAppItemType::mvLineSeries:                  return "add_line_series";
    case mvAppItemType::mvScatterSeries:               return "add_scatter_series";
    case mvAppItemType::mvStemSeries:                  return "add_stem_series";
    case mvAppItemType::mvStairSeries:                 return "add_stair_series";
    case mvAppItemType::mvBarSeries:                   return "add_bar_series";
    case mvAppItemType::mvGroupBarSeries:              return "add_group_bar_series";
    case mvAppItemType::mvErrorSeries:                 return "add_error_series";
    case mvAppItemType::mvCustomSeries:                return "add_custom_series";
    case mvAppItemType::mvInfLineSeries:               return "add_inf_line_series";
    case mvAppItemType::mvHeatSeries:                  return "add_heat_series";
    case mvAppItemType::mvImageSeries:                 return "add_image_series";
    case mvAppItemType::mvPieSeries:                   return "add_pie_series";
    case mvAppItemType::mvShadeSeries:                 return "add_shade_series";
    case mvAppItemType::mvLabelSeries:                 return "add_text_point";
    case mvAppItemType::mvHistogramSeries:             return "add_histogram_series";
    case mvAppItemType::mv2dHistogramSeries:           return "add_2d_histogram_series";
    case mvAppItemType::mvCandleSeries:                return "add_candle_series";
    case mvAppItemType::mvAreaSeries:                  return "add_area_series";
    case mvAppItemType::mvColorMapScale:               return "add_colormap_scale";
    case mvAppItemType::mvSlider3D:                    return "add_3d_slider";
    case mvAppItemType::mvKnobFloat:                   return "add_knob_float";
    case mvAppItemType::mvLoadingIndicator:            return "add_loading_indicator";
    case mvAppItemType::mvNodeLink:                    return "add_node_link";
    case mvAppItemType::mvTextureRegistry:             return "add_texture_registry";
    case mvAppItemType::mvStaticTexture:               return "add_static_texture";
    case mvAppItemType::mvDynamicTexture:              return "add_dynamic_texture";
    case mvAppItemType::mvStage:                       return "add_stage";
    case mvAppItemType::mvDrawLayer:                   return "add_draw_layer";
    case mvAppItemType::mvViewportDrawlist:            return "add_viewport_drawlist";
    case mvAppItemType::mvFileExtension:               return "add_file_extension";
    case mvAppItemType::mvPlotLegend:                  return "add_plot_legend";
    case mvAppItemType::mvPlotAxis:                    return "add_plot_axis";
    case mvAppItemType::mvHandlerRegistry:             return "add_handler_registry";
    case mvAppItemType::mvKeyDownHandler:              return "add_key_down_handler";
    case mvAppItemType::mvKeyPressHandler:             return "add_key_press_handler";
    case mvAppItemType::mvKeyReleaseHandler:           return "add_key_release_handler";
    case mvAppItemType::mvMouseMoveHandler:            return "add_mouse_move_handler";
    case mvAppItemType::mvMouseWheelHandler:           return "add_mouse_wheel_handler";
    case mvAppItemType::mvMouseClickHandler:           return "add_mouse_click_handler";
    case mvAppItemType::mvMouseDoubleClickHandler:     return "add_mouse_double_click_handler";
    case mvAppItemType::mvMouseDownHandler:            return "add_mouse_down_handler";
    case mvAppItemType::mvMouseReleaseHandler:         return "add_mouse_release_handler";
    case mvAppItemType::mvMouseDragHandler:            return "add_mouse_drag_handler";
    case mvAppItemType::mvHoverHandler:                return "add_item_hover_handler";
    case mvAppItemType::mvActiveHandler:               return "add_item_active_handler";
    case mvAppItemType::mvFocusHandler:                return "add_item_focus_handler";
    case mvAppItemType::mvVisibleHandler:              return "add_item_visible_handler";
    case mvAppItemType::mvEditedHandler:               return "add_item_edited_handler";
    case mvAppItemType::mvActivatedHandler:            return "add_item_activated_handler";
    case mvAppItemType::mvDeactivatedHandler:          return "add_item_deactivated_handler";
    case mvAppItemType::mvDeactivatedAfterEditHandler: return "add_item_deactivated_after_edit_handler";
    case mvAppItemType::mvToggledOpenHandler:          return "add_item_toggled_open_handler";
    case mvAppItemType::mvClickedHandler:              return "add_item_clicked_handler";
    case mvAppItemType::mvDoubleClickedHandler:        return "add_item_double_clicked_handler";
    case mvAppItemType::mvDragPayload:                 return "add_drag_payload";
    case mvAppItemType::mvResizeHandler:               return "add_item_resize_handler";
    case mvAppItemType::mvFont:                        return "add_font";
    case mvAppItemType::mvFontRegistry:                return "add_font_registry";
    case mvAppItemType::mvTheme:                       return "add_theme";
    case mvAppItemType::mvThemeColor:                  return "add_theme_color";
    case mvAppItemType::mvThemeStyle:                  return "add_theme_style";
    case mvAppItemType::mvThemeComponent:              return "add_theme_component";
    case mvAppItemType::mvFontRangeHint:               return "add_font_range_hint";
    case mvAppItemType::mvFontRange:                   return "add_font_range";
    case mvAppItemType::mvFontChars:                   return "add_font_chars";
    case mvAppItemType::mvCharRemap:                   return "add_char_remap";
    case mvAppItemType::mvValueRegistry:               return "add_value_registry";
    case mvAppItemType::mvIntValue:                    return "add_int_value";
    case mvAppItemType::mvFloatValue:                  return "add_float_value";
    case mvAppItemType::mvFloat4Value:                 return "add_float4_value";
    case mvAppItemType::mvInt4Value:                   return "add_int4_value";
    case mvAppItemType::mvBoolValue:                   return "add_bool_value";
    case mvAppItemType::mvStringValue:                 return "add_string_value";
    case mvAppItemType::mvDoubleValue:                 return "add_double_value";
    case mvAppItemType::mvDouble4Value:                return "add_double4_value";
    case mvAppItemType::mvColorValue:                  return "add_color_value";
    case mvAppItemType::mvFloatVectValue:              return "add_float_vect_value";
    case mvAppItemType::mvSeriesValue:                 return "add_series_value";
    case mvAppItemType::mvRawTexture:                  return "add_raw_texture";
    case mvAppItemType::mvSubPlots:                    return "add_subplots";
    case mvAppItemType::mvColorMap:                    return "add_colormap";
    case mvAppItemType::mvColorMapRegistry:            return "add_colormap_registry";
    case mvAppItemType::mvColorMapButton:              return "add_colormap_button";
    case mvAppItemType::mvColorMapSlider:              return "add_colormap_slider";
    case mvAppItemType::mvTemplateRegistry:            return "add_template_registry";
    case mvAppItemType::mvTableCell:                   return "add_table_cell";
    case mvAppItemType::mvItemHandlerRegistry:         return "add_item_handler_registry";
    default:
    {
        assert(false);
        return "no command";
    }
    }
}

#define MV_BASE_COL_bgColor mvColor(37, 37, 38, 255)
#define MV_BASE_COL_lightBgColor mvColor(82, 82, 85, 255)
#define MV_BASE_COL_veryLightBgColor mvColor(90, 90, 95, 255)
#define MV_BASE_COL_panelColor mvColor(51, 51, 55, 255)
#define MV_BASE_COL_panelHoverColor mvColor(29, 151, 236, 103)
#define MV_BASE_COL_panelActiveColor mvColor(0, 119, 200, 153)
#define MV_BASE_COL_textColor mvColor(255, 255, 255, 255)
#define MV_BASE_COL_textDisabledColor mvColor(151, 151, 151, 255)
#define MV_BASE_COL_borderColor mvColor(78, 78, 78, 255)
#define mvImGuiCol_Text MV_BASE_COL_textColor
#define mvImGuiCol_TextSelectedBg MV_BASE_COL_panelActiveColor
#define mvImGuiCol_WindowBg MV_BASE_COL_bgColor
#define mvImGuiCol_ChildBg MV_BASE_COL_bgColor
#define mvImGuiCol_PopupBg MV_BASE_COL_bgColor
#define mvImGuiCol_Border MV_BASE_COL_borderColor
#define mvImGuiCol_BorderShadow MV_BASE_COL_borderColor
#define mvImGuiCol_FrameBg MV_BASE_COL_panelColor
#define mvImGuiCol_FrameBgHovered MV_BASE_COL_panelHoverColor
#define mvImGuiCol_FrameBgActive MV_BASE_COL_panelActiveColor
#define mvImGuiCol_TitleBg MV_BASE_COL_bgColor
#define mvImGuiCol_TitleBgActive mvColor(15, 86, 135, 255)
#define mvImGuiCol_TitleBgCollapsed MV_BASE_COL_bgColor
#define mvImGuiCol_MenuBarBg MV_BASE_COL_panelColor
#define mvImGuiCol_ScrollbarBg MV_BASE_COL_panelColor
#define mvImGuiCol_ScrollbarGrab MV_BASE_COL_lightBgColor
#define mvImGuiCol_ScrollbarGrabHovered MV_BASE_COL_veryLightBgColor
#define mvImGuiCol_ScrollbarGrabActive MV_BASE_COL_veryLightBgColor
#define mvImGuiCol_CheckMark MV_BASE_COL_panelActiveColor
#define mvImGuiCol_SliderGrab MV_BASE_COL_panelHoverColor
#define mvImGuiCol_SliderGrabActive MV_BASE_COL_panelActiveColor
#define mvImGuiCol_Button MV_BASE_COL_panelColor
#define mvImGuiCol_ButtonHovered MV_BASE_COL_panelHoverColor
#define mvImGuiCol_ButtonActive MV_BASE_COL_panelActiveColor
#define mvImGuiCol_Header MV_BASE_COL_panelColor
#define mvImGuiCol_HeaderHovered MV_BASE_COL_panelHoverColor
#define mvImGuiCol_HeaderActive MV_BASE_COL_panelActiveColor
#define mvImGuiCol_Separator MV_BASE_COL_borderColor
#define mvImGuiCol_SeparatorHovered MV_BASE_COL_borderColor
#define mvImGuiCol_SeparatorActive MV_BASE_COL_borderColor
#define mvImGuiCol_ResizeGrip MV_BASE_COL_bgColor
#define mvImGuiCol_ResizeGripHovered MV_BASE_COL_panelColor
#define mvImGuiCol_Tab MV_BASE_COL_panelColor
#define mvImGuiCol_TabHovered MV_BASE_COL_panelHoverColor
#define mvImGuiCol_TabActive MV_BASE_COL_panelActiveColor
#define mvImGuiCol_TabUnfocused MV_BASE_COL_panelColor
#define mvImGuiCol_TabUnfocusedActive MV_BASE_COL_panelActiveColor
#define mvImGuiCol_PlotLines MV_BASE_COL_panelActiveColor
#define mvImGuiCol_PlotLinesHovered MV_BASE_COL_panelHoverColor
#define mvImGuiCol_PlotHistogram MV_BASE_COL_panelActiveColor
#define mvImGuiCol_PlotHistogramHovered MV_BASE_COL_panelHoverColor
#define mvImGuiCol_DragDropTarget mvColor(255, 255, 0, 179)
#define mvImGuiCol_NavHighlight MV_BASE_COL_bgColor
#define mvImGuiCol_NavWindowingHighlight mvColor(255, 255, 255, 179)
#define mvImGuiCol_NavWindowingDimBg mvColor(204, 204, 204, 51)
#define mvImGuiCol_ModalWindowDimBg mvColor(37, 37, 38, 150)
#define mvImGuiCol_TableHeaderBg mvColor(48, 48, 51, 255)
#define mvImGuiCol_TableBorderStrong mvColor(79, 79, 89, 255)
#define mvImGuiCol_TableBorderLight mvColor(59, 59, 64, 255)
#define mvImGuiCol_TableRowBg mvColor(0, 0, 0, 0)
#define mvImGuiCol_TableRowBgAlt mvColor(255, 255, 255, 15)