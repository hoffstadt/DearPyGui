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
#include "mvPythonTranslator.h"
#include "cpp.hint"
#include "mvDefaultTheme.h"
#include <implot_internal.h>
#include "mvAppItemTypes.inc"

namespace Marvel {

    // forward declarations
    class mvThemeManager;
    class mvAppItem;

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
        Int, Int4, Float, Float4, FloatVect, 
        Double, Double4, DoubleVect,
        Series, Bool, String, UUID,
        Time, Color, Texture
    };

    enum ItemDescriptionFlags
    {
        MV_ITEM_DESC_DEFAULT     = 0,
        MV_ITEM_DESC_ROOT        = 1 << 1,
        MV_ITEM_DESC_CONTAINER   = 1 << 2,
        MV_ITEM_DESC_HANDLER     = 1 << 3 // todo: rename descriptively
    };

    enum class mvLibType {
        MV_IMGUI = 0,
        MV_IMPLOT = 1,
        MV_IMNODES = 2
    };

    struct ScopedID
    {
        ScopedID(void* id) { ImGui::PushID(id); }
        ScopedID(mvUUID id) { ImGui::PushID((int)id); }
        ~ScopedID() { ImGui::PopID(); }
    };

    mvRef<mvAppItem>                                CreateEntity                    (mvAppItemType type, mvUUID id);
    i32                                             GetEntityDesciptionFlags        (mvAppItemType type);
    i32                                             GetEntityTargetSlot             (mvAppItemType type);
    StorageValueTypes                               GetEntityValueType              (mvAppItemType type);
    const char*                                     GetEntityTypeString             (mvAppItemType type);
    i32                                             GetApplicableState              (mvAppItemType type);
    const std::vector<std::pair<std::string, i32>>& GetAllowableParents             (mvAppItemType type);
    const std::vector<std::pair<std::string, i32>>& GetAllowableChildren            (mvAppItemType type);
    mvRef<mvAppItem>&                               GetClassThemeComponent          (mvAppItemType type);
    mvRef<mvAppItem>&                               GetDisabledClassThemeComponent  (mvAppItemType type);
    mvPythonParser                                  GetEntityParser                 (mvAppItemType type);

    //-----------------------------------------------------------------------------
    // mvAppItem
    //-----------------------------------------------------------------------------
    class mvAppItem
    {

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
        virtual void alternativeCustomAction(void* data = nullptr) {};

        //-----------------------------------------------------------------------------
        // shows information in either the debug window or a separate window
        //-----------------------------------------------------------------------------
        virtual void renderSpecificDebugInfo() {} // for the debug tool

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
        void applyTemplate    (mvAppItem* item);

        // used by derived items to register their arguments
        virtual void handleSpecificRequiredArgs  (PyObject* args)  {}
        virtual void handleSpecificPositionalArgs(PyObject* args)  {}
        virtual void handleSpecificKeywordArgs   (PyObject* dict)  {} // called by handleKeywordArgs
        virtual void getSpecificConfiguration    (PyObject* dict)  {}
        virtual void applySpecificTemplate       (mvAppItem* item) {}

        //-----------------------------------------------------------------------------
        // These methods can be optionally overridden if your widget needs to be
        // notified when children are added/removed (i.e. tables, node editor)
        //-----------------------------------------------------------------------------
        virtual void onChildAdd    (mvRef<mvAppItem> item) {}
        virtual void onChildRemoved(mvRef<mvAppItem> item) {}
        virtual void onChildrenRemoved() {}
        virtual void onBind(mvAppItem* item) {}

        //-----------------------------------------------------------------------------
        // callbacks
        //-----------------------------------------------------------------------------
        [[nodiscard]] PyObject* getCallback(b8 ignore_enabled = true);  // returns the callback. If ignore_enable false and item is disabled then no callback will be returned.
       
        //-----------------------------------------------------------------------------
        // config setters
        //-----------------------------------------------------------------------------
        virtual void setDataSource(mvUUID value);

        mvAppItem*       getChild(mvUUID uuid);      // will return nullptr if not found
        mvRef<mvAppItem> getChildRef(mvUUID uuid);      // will return nullptr if not found

        // runtime modifications
        bool             addItem(mvRef<mvAppItem> item);
        bool             addRuntimeChild(mvUUID parent, mvUUID before, mvRef<mvAppItem> item);
        bool             addChildAfter(mvUUID prev, mvRef<mvAppItem> item);
        bool             deleteChild(mvUUID uuid);
        bool             moveChildUp(mvUUID uuid);
        bool             moveChildDown(mvUUID uuid);
        mvRef<mvAppItem> stealChild(mvUUID uuid); // steals a child (used for moving)
       
    public:

        mvAppItemType  _type = mvAppItemType::None;
        mvUUID         _uuid = 0;
        std::string    _internalLabel; // label passed into imgui
        mvAppItem*     _parentPtr = nullptr;
        mvAppItemState _state;
        i32            _location = -1;
        b8             _showDebug = false;
        
        // next frame triggers
        b8 _focusNextFrame = false;
        b8 _triggerAlternativeAction = false;
        b8 _shownLastFrame = false;
        b8 _hiddenLastFrame = false;
        b8 _enabledLastFrame = false;
        b8 _disabledLastFrame = false;

        // previous frame cache
        ImVec2 _previousCursorPos = { 0.0f, 0.0f };

        // dirty flags
        b8 _dirty_size = true;
        b8 _dirtyPos = false;

        // slots
        //   * 0 : mvFileExtension, mvFontRangeHint, mvNodeLink, mvAnnotation
        //         mvDragLine, mvDragPoint, mvLegend, mvTableColumn
        //   * 1 : Most widgets
        //   * 2 : Draw Commands
        //   * 3 : mvDragPayload
        std::vector<mvRef<mvAppItem>> _children[4] = { {}, {}, {}, {} };

        // item handler registry
        mvRef<mvAppItem> _handlerRegistry = nullptr;

        // font
        mvRef<mvAppItem> _font = nullptr;

        // theme
        mvRef<mvAppItem> _theme = nullptr;

        // drag & drop
        PyObject* _dragCallback = nullptr;
        PyObject* _dropCallback = nullptr;
        std::string _payloadType = "$$DPG_PAYLOAD";

        // config
        mvUUID      _source = 0;
        std::string _specifiedLabel;
        mvUUID      _parent = 0;
        mvUUID      _before = 0;
        std::string _filter;
        i32         _width = 0;
        i32         _height = 0;
        f32         _indent = -1.0f;
        b8          _show = true;
        b8          _enabled = true;
        PyObject*   _callback = nullptr;
        PyObject*   _user_data = nullptr;
        b8          _tracked = false;
        f32         _trackOffset = 0.5f; // 0.0f:top, 0.5f:center, 1.0f:bottom
        b8          _searchLast = false;
        b8          _searchDelayed = false;
        b8          _useInternalLabel = true; // when false, will use specificed label
        std::string _alias;

        // only for draw cmds and layers (unfortunately this is the best place
        // to put it for the moment.
        mvMat4 _transform = mvIdentityMat4();

        // only used by nodes
        mvMat4 _appliedTransform = mvIdentityMat4();

        // only used by draw items
        long    _cullMode = 0; // mvCullMode_None
        b8      _perspectiveDivide = false;
        b8      _depthClipping = false;
        f32     _clipViewport[6] = { 0.0f, 0.0f, 1.0f, 1.0f, -1.0f, 1.0f }; // top leftx, top lefty, width, height, min depth, maxdepth

    };

    inline b8 mvClipPoint(f32 clipViewport[6], mvVec4& point)
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
        case mvAppItemType::mvSliderInt:                   return "add_slider_int";
        case mvAppItemType::mvFilterSet:                   return "add_filter_set";
        case mvAppItemType::mvDragFloat:                   return "add_drag_float";
        case mvAppItemType::mvDragInt:                     return "add_drag_int";
        case mvAppItemType::mvInputFloat:                  return "add_input_float";
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
        case mvAppItemType::mvDragIntMulti:                return "add_drag_intx";
        case mvAppItemType::mvSliderFloatMulti:            return "add_slider_floatx";
        case mvAppItemType::mvSliderIntMulti:              return "add_slider_intx";
        case mvAppItemType::mvInputIntMulti:               return "add_input_intx";
        case mvAppItemType::mvInputFloatMulti:             return "add_input_floatx";
        case mvAppItemType::mvDragPoint:                   return "add_drag_point";
        case mvAppItemType::mvDragLine:                    return "add_drag_line";
        case mvAppItemType::mvAnnotation:                  return "add_plot_annotation";
        case mvAppItemType::mvLineSeries:                  return "add_line_series";
        case mvAppItemType::mvScatterSeries:               return "add_scatter_series";
        case mvAppItemType::mvStemSeries:                  return "add_stem_series";
        case mvAppItemType::mvStairSeries:                 return "add_stair_series";
        case mvAppItemType::mvBarSeries:                   return "add_bar_series";
        case mvAppItemType::mvErrorSeries:                 return "add_error_series";
        case mvAppItemType::mvVLineSeries:                 return "add_vline_series";
        case mvAppItemType::mvHLineSeries:                 return "add_hline_series";
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
}
