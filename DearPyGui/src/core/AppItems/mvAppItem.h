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

namespace Marvel {

    // forward declarations
    class mvThemeManager;

    //-----------------------------------------------------------------------------
    // mvAppItemType
    //-----------------------------------------------------------------------------
    enum class mvAppItemType
    {
        None = 0, mvSpacing, mvSameLine, mvInputText, mvButton,
        mvRadioButton, mvTabBar, mvTab, mvImage, mvMenuBar, mvViewportMenuBar,
        mvMenu, mvMenuItem, mvGroup, mvChild,
        mvSliderFloat, mvSliderInt, mvFilterSet,
        mvDragFloat, mvDragInt, mvInputFloat,
        mvInputInt, mvColorEdit, mvClipper,
        mvColorPicker, mvTooltip, mvCollapsingHeader,
        mvSeparator, mvCheckbox, mvListbox, mvText, mvCombo,
        mvPlot, mvSimplePlot, mvDrawlist, mvWindowAppItem,
        mvSelectable, mvTreeNode, mvProgressBar, mvDummy,
        mvImageButton, mvTimePicker, mvDatePicker, mvColorButton,
        mvFileDialog, mvTabButton,
        mvNodeEditor, mvNode, mvNodeAttribute,
        mvTable, mvTableColumn, mvTableNextColumn, mvTableRow,
        mvDrawLine, mvDrawArrow, mvDrawTriangle,
        mvDrawCircle, mvDrawEllipse, mvDrawBezierCubic, mvDrawBezierQuadratic,
        mvDrawQuad, mvDrawRect, mvDrawText, mvDrawPolygon, mvDrawPolyline,
        mvDrawImage, mvDragFloatMulti, mvDragIntMulti, mvSliderFloatMulti,
        mvSliderIntMulti, mvInputIntMulti, mvInputFloatMulti,
        mvDragPoint, mvDragLine, mvAnnotation, mvLineSeries,
        mvScatterSeries, mvStemSeries, mvStairSeries, mvBarSeries,
        mvErrorSeries, mvVLineSeries, mvHLineSeries, mvHeatSeries,
        mvImageSeries, mvPieSeries, mvShadeSeries, mvLabelSeries,
        mvHistogramSeries, mv2dHistogramSeries,
        mvCandleSeries, mvAreaSeries, mvColorMapScale, mvSlider3D,
        mvKnobFloat, mvLoadingIndicator, mvNodeLink, 
        mvTextureRegistry, mvStaticTexture, mvDynamicTexture,
        mvStagingContainer, mvDrawLayer, mvViewportDrawlist,
        mvFileExtension, mvPlotLegend, mvPlotAxis,
        mvHandlerRegistry, mvKeyDownHandler, mvKeyPressHandler,
        mvKeyReleaseHandler, mvMouseMoveHandler, mvMouseWheelHandler,
        mvMouseClickHandler, mvMouseDoubleClickHandler, mvMouseDownHandler,
        mvMouseReleaseHandler, mvMouseDragHandler, 
        mvHoverHandler, mvActiveHandler, mvFocusHandler, mvVisibleHandler,
        mvEditedHandler, mvActivatedHandler, mvDeactivatedHandler, mvDeactivatedAfterEditHandler,
        mvToggledOpenHandler, mvClickedHandler, mvDragPayload, mvResizeHandler,
        mvFont, mvFontRegistry, mvTheme, mvThemeColor, mvThemeStyle,
        mvFontRangeHint, mvFontRange, mvFontChars, mvCharRemap,
        mvValueRegistry, mvIntValue, mvFloatValue, mvFloat4Value,
        mvInt4Value, mvBoolValue, mvStringValue, mvDoubleValue, mvDouble4Value,
        mvColorValue, mvFloatVectValue, mvSeriesValue, mvRawTexture, mvSubPlots,
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
        MV_ITEM_DESC_AFTER       = 1 << 3,
        MV_ITEM_DESC_HANDLER     = 1 << 4,
        MV_ITEM_DESC_ALWAYS_DRAW = 1 << 5,
    };

    enum CommonParserArgs
    {
        MV_PARSER_ARG_ID            = 1 << 1,
        MV_PARSER_ARG_WIDTH         = 1 << 2,
        MV_PARSER_ARG_HEIGHT        = 1 << 3,
        MV_PARSER_ARG_INDENT        = 1 << 4,
        MV_PARSER_ARG_PARENT        = 1 << 5,
        MV_PARSER_ARG_BEFORE        = 1 << 6,
        MV_PARSER_ARG_SOURCE        = 1 << 7,
        MV_PARSER_ARG_CALLBACK      = 1 << 8,
        MV_PARSER_ARG_SHOW          = 1 << 9,
        MV_PARSER_ARG_ENABLED       = 1 << 10,
        MV_PARSER_ARG_POS           = 1 << 11,
        MV_PARSER_ARG_DROP_CALLBACK = 1 << 12,
        MV_PARSER_ARG_DRAG_CALLBACK = 1 << 13,
        MV_PARSER_ARG_PAYLOAD_TYPE  = 1 << 14,
        MV_PARSER_ARG_TRACKED       = 1 << 15,
        MV_PARSER_ARG_FILTER        = 1 << 16,
        MV_PARSER_ARG_SEARCH_DELAY  = 1 << 17,
    };

    enum class mvLibType {
        MV_IMGUI = 0,
        MV_IMPLOT = 1,
        MV_IMNODES = 2
    };

    // todo: remove this nonsense (relic of CPP interface idea)
    using mvValueVariant = std::variant<
        std::shared_ptr<mvUUID>,
        std::shared_ptr<int>,
        std::shared_ptr<float>,
        std::shared_ptr<double>,
        std::shared_ptr<std::array<int, 4>>,
        std::shared_ptr<std::array<float, 4>>,
        std::shared_ptr<std::array<double, 4>>,
        std::shared_ptr<std::vector<int>>,
        std::shared_ptr<std::vector<float>>,
        std::shared_ptr<std::vector<double>>,
        std::shared_ptr<std::vector<std::vector<float>>>,
        std::shared_ptr<std::vector<std::vector<double>>>,
        std::shared_ptr<bool>,
        std::shared_ptr<std::string>,
        std::shared_ptr<tm>,
        std::shared_ptr<ImPlotTime>,
        void*>;

    template<int item_type> 
    struct mvItemTypeMap {};

    template<typename T> 
    struct mvItemTypeReverseMap{};

    //-----------------------------------------------------------------------------
    // mvAppItem
    //-----------------------------------------------------------------------------
    class mvAppItem
    {

        friend class mvApp;
        friend class mvItemRegistry;
        friend class mvAppItemState;
        friend class mvLayoutWindow;
       
    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_CREATE_COMMAND(get_item_configuration);
        MV_CREATE_COMMAND(get_item_state);
        MV_CREATE_COMMAND(get_item_info);
        MV_CREATE_COMMAND(configure_item);
        MV_CREATE_COMMAND(get_item_types);
        MV_CREATE_COMMAND(get_value);
        MV_CREATE_COMMAND(get_values);
        MV_CREATE_COMMAND(set_value);
        MV_CREATE_COMMAND(focus_item);
        MV_CREATE_COMMAND(reset_pos);
        MV_CREATE_COMMAND(set_item_children);
        MV_CREATE_COMMAND(set_item_font);
        MV_CREATE_COMMAND(set_item_theme);
        MV_CREATE_COMMAND(set_item_disabled_theme);
        MV_CREATE_COMMAND(set_item_type_theme);
        MV_CREATE_COMMAND(set_item_type_disabled_theme);

        MV_START_COMMANDS
            MV_ADD_COMMAND(get_item_types);
            MV_ADD_COMMAND(get_item_configuration);
            MV_ADD_COMMAND(get_item_state);
            MV_ADD_COMMAND(get_item_info);
            MV_ADD_COMMAND(configure_item);
            MV_ADD_COMMAND(get_value);
            MV_ADD_COMMAND(get_values);
            MV_ADD_COMMAND(set_value);
            MV_ADD_COMMAND(focus_item);
            MV_ADD_COMMAND(reset_pos);
            MV_ADD_COMMAND(set_item_children);
            MV_ADD_COMMAND(set_item_font);
            MV_ADD_COMMAND(set_item_theme);
            MV_ADD_COMMAND(set_item_disabled_theme);
            MV_ADD_COMMAND(set_item_type_theme);
            MV_ADD_COMMAND(set_item_type_disabled_theme);
        MV_END_COMMANDS

        //-----------------------------------------------------------------------------
        // Helpers
        //-----------------------------------------------------------------------------
        static bool DoesItemHaveFlag(mvAppItem* item, int flag);

        // retrieves parent, before, uuid from user input when creating an item
        static std::pair<mvUUID, mvUUID> GetNameFromArgs(mvUUID& name, PyObject* args, PyObject* kwargs);

        // adds the common app item arguments (label, id, etc.)
        static void AddCommonArgs(mvPythonParser& parser, CommonParserArgs args);

    protected:

            struct ScopedID
            {
                ScopedID(void* id) { ImGui::PushID(id); }
                ScopedID(mvUUID id) { ImGui::PushID((int)id); }
                ~ScopedID() { ImGui::PopID(); }
            };

    public:

        mvAppItem(mvUUID uuid);
        mvAppItem(const mvAppItem& other) = delete; // copy constructor
        mvAppItem(mvAppItem&& other)      = delete; // move constructor

        virtual ~mvAppItem();

        //-----------------------------------------------------------------------------
        // These methods are overridden through the use of:
        //   - MV_REGISTER_WIDGET
        //   - MV_APPLY_WIDGET_REGISTRATION
        //-----------------------------------------------------------------------------
        [[nodiscard]] virtual mvRef<mvAppItem>  getClassDisabledTheme() const = 0;
        [[nodiscard]] virtual mvRef<mvAppItem>  getClassTheme() const = 0;
        [[nodiscard]] virtual mvAppItemType     getType      () const { return mvAppItemType::None; } // should be pure, see #1071
        [[nodiscard]] virtual int               getDescFlags () const = 0;
        [[nodiscard]] virtual int               getTarget    () const = 0; // which child slot
        [[nodiscard]] virtual StorageValueTypes getValueType () const = 0;

        // actual immediate mode drawing instructions
        virtual bool preDraw();
        virtual void draw(ImDrawList* drawlist, float x, float y) = 0;
        virtual void postDraw();

        //-----------------------------------------------------------------------------
        // usually used for iterating through items and performing an action
        // other than drawing.
        //-----------------------------------------------------------------------------
        virtual void customAction() {};
        virtual void alternativeCustomAction() {};

        //-----------------------------------------------------------------------------
        // These methods handle setting the widget's value using PyObject*'s or
        // returning the actual value. These are mostly overridden by the
        // mvTypeBase classes
        //-----------------------------------------------------------------------------
        virtual mvValueVariant getValue() { return nullptr; }
        virtual PyObject*      getPyValue() { return GetPyNone(); }
        virtual void           setPyValue(PyObject* value) { }

        //-----------------------------------------------------------------------------
        // These methods handle are used by the item registry:
        //   - isParentCompatible -> will the parent accept the current item
        //   - canChildBeAdded -> will the current item accept the incoming child
        //-----------------------------------------------------------------------------
        virtual bool isParentCompatible(mvAppItemType type) { return true; }
        virtual bool canChildBeAdded   (mvAppItemType type) { return true; }

        // used to check arguments, get/set configurations
        void getItemInfo      (PyObject* dict);
        void checkArgs        (PyObject* args, PyObject* kwargs);    
        void handleKeywordArgs(PyObject* dict);  // python dictionary acts as an out parameter 
        void getConfiguration (PyObject* dict);

        // used by derived items to register their arguments
        virtual void handleSpecificRequiredArgs  (PyObject* args) {}
        virtual void handleSpecificPositionalArgs(PyObject* args) {}
        virtual void handleSpecificKeywordArgs   (PyObject* dict) {} // called by handleKeywordArgs
        virtual void getSpecificConfiguration    (PyObject* dict) {}

        //-----------------------------------------------------------------------------
        // These methods can be optionally overridden if your widget needs to be
        // notified when children are added/removed (i.e. tables, node editor)
        //-----------------------------------------------------------------------------
        virtual void onChildAdd    (mvRef<mvAppItem> item) {}
        virtual void onChildRemoved(mvRef<mvAppItem> item) {}
        virtual void onChildrenRemoved() {}

        //-----------------------------------------------------------------------------
        // callbacks
        //-----------------------------------------------------------------------------
        void                    setCallback(PyObject* callback);
        [[nodiscard]] PyObject* getCallback(bool ignore_enabled = true);  // returns the callback. If ignore_enable false and item is disabled then no callback will be returned.
        [[nodiscard]] PyObject* getCallbackData() { return _user_data; }

        //-----------------------------------------------------------------------------
        // drag & drop
        //-----------------------------------------------------------------------------
        void                    setPayloadType (const std::string& payloadType);
        void                    setDragCallback(PyObject* callback);
        void                    setDropCallback(PyObject* callback);
        [[nodiscard]] PyObject* getDragCallback() { return _dragCallback; }
        [[nodiscard]] PyObject* getDropCallback() { return _dropCallback; }

        //-----------------------------------------------------------------------------
        // dirty flags
        //-----------------------------------------------------------------------------
        bool isPosDirty() const { return _dirtyPos; }

        virtual void                        focus          () { _focusNextFrame = true; }
        virtual void                        unfocus        () { _focusNextFrame = false; }
        virtual void                        hide           () { _show = false; }
        virtual void                        show           () { _show = true; }
        void                                setCallbackData(PyObject* data);
        void                                updateLocations();

        std::vector<mvRef<mvAppItem>>&      getChildren(int slot);
        void                                setChildren(int slot, std::vector<mvRef<mvAppItem>> children);

        [[nodiscard]] bool                  isShown        () const { return _show; }


        mvAppItemState&                     getState       () { return _state; } 
        mvAppItem*                          getParent() { return _parentPtr; }
        bool                                isEnabled() const { return _enabled; }
        int                                 getWidth() const { return _width; }
        int                                 getHeight() const { return _height; }
        mvUUID                              getUUID() const { return _uuid; }
        const std::string&                  getFilter() const { return _filter; }
        const std::string&                  getLabel() const { return _label; }
        const std::string&                  getSpecifiedLabel() const { return _specificedlabel; }
        mvAppItem*                          getRoot() const;
        int                                 getLocation() const { return _location; }
        bool                                isAltCustomActionRequested() const { return _triggerAlternativeAction; }
        bool                                isTracked() const { return _tracked; }
        float                               getTrackOffset() const { return _trackOffset; }
        virtual void                        setWidth(int width) { _width = width; }
        virtual void                        setHeight(int height) { _height = height; }
        virtual void                        setEnabled   (bool value)              { _enabled = value; }
        virtual void                        setDataSource(mvUUID value)            { _source = value; }
        virtual void                        setLabel     (const std::string& value); 
        void                                setFilter    (const std::string& value); 
        void                                setPos       (const ImVec2& pos);
        void                                registerWindowFocusing(); // only useful for imgui window types
        bool                                shouldFocusNextFrame() const { return _focusNextFrame; }

    private:

        mvAppItem*       getChild(mvUUID uuid);      // will return nullptr if not found
        mvRef<mvAppItem> getChildRef(mvUUID uuid);      // will return nullptr if not found

        // runtime modifications
        bool             addItem(mvRef<mvAppItem> item);
        bool             addRuntimeChild(mvUUID parent, mvUUID before, mvRef<mvAppItem> item);
        bool             addChildAfter(mvUUID prev, mvRef<mvAppItem> item);
        bool             deleteChild(mvUUID uuid);
        void             deleteChildren(int slot = -1);
        bool             moveChildUp(mvUUID uuid);
        bool             moveChildDown(mvUUID uuid);
        void             resetState();
        mvRef<mvAppItem> stealChild(mvUUID uuid); // steals a child (used for moving)

       
    protected:

        mvUUID         _uuid = 0;
        mvAppItemState _state;
        bool           _focusNextFrame = false;
        bool           _dirtyPos = false;
        ImVec2         _previousCursorPos = { 0.0f, 0.0f };
        int            _location = -1;
        std::string    _label; // internal imgui label
        bool           _triggerAlternativeAction = false;
        mvAppItem*     _parentPtr = nullptr;

        // slots
        //   * 0 : mvFileExtension, mvFontRangeHint, mvNodeLink, mvAnnotation
        //         mvDragLine, mvDragPoint, mvLegend, mvTableColumn
        //   * 1 : Most widgets
        //   * 2 : Draw Commands
        //   * 3 : Widget Handlers
        //   * 4 : mvDragPayload
        std::vector<mvRef<mvAppItem>> _children[5] = { {}, {}, {}, {}, {} };

        // font
        mvRef<mvAppItem> _font = nullptr;

        // theme
        mvRef<mvAppItem> _theme = nullptr;
        mvRef<mvAppItem> _disabledTheme = nullptr;

        // config
        mvUUID      _source = 0;
        std::string _specificedlabel;
        mvUUID      _parent = 0;
        mvUUID      _before = 0;
        std::string _filter = "";
        int         _width = 0;
        int         _height = 0;
        float       _indent = -1.0f;
        int         _windowPosx = 0;
        int         _windowPosy = 0;
        int         _posx = 0;
        int         _posy = 0;
        bool        _show = true;
        bool        _enabled = true;
        PyObject*   _callback = nullptr;
        PyObject*   _user_data = nullptr;
        bool        _tracked = false;
        float       _trackOffset = 0.5f; // 0.0f:top, 0.5f:center, 1.0f:bottom
        bool        _searchLast = false;
        bool        _searchDelayed = false;

        // drag & drop
        PyObject*   _dragCallback = nullptr;
        PyObject*   _dropCallback = nullptr;
        std::string _payloadType = "$$DPG_PAYLOAD";

    };

}