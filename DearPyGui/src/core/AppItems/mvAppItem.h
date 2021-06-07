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
#include "mvThemeColorGroup.h"
#include "mvThemeStyleGroup.h"

// forward declarations
struct ImPlotTime;

namespace Marvel {

    // forward declarations
    class mvThemeManager;

    //-----------------------------------------------------------------------------
    // mvAppItemType
    //-----------------------------------------------------------------------------
    enum class mvAppItemType
    {
        None = 0, mvSpacing, mvSameLine, mvInputText, mvButton,
        mvRadioButton, mvTabBar, mvTab, mvImage, mvMenuBar,
        mvMenu, mvMenuItem, mvGroup, mvChild,
        mvSliderFloat, mvSliderInt, mvFilterSet,
        mvDragFloat, mvDragInt, mvInputFloat,
        mvInputInt, mvColorEdit, mvClipper,
        mvColorPicker, mvTooltip, mvCollapsingHeader,
        mvSeparator, mvCheckbox, mvListbox, mvText, mvCombo,
        mvPlot, mvSimplePlot, mvDrawlist, mvWindowAppItem,
        mvPopup, mvSelectable, mvTreeNode, mvProgressBar, mvDummy,
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
        mvTextureContainer, mvStaticTexture, mvDynamicTexture,
        mvStagingContainer, mvDrawLayer, mvViewportDrawlist,
        mvFileExtension, mvPlotLegend, mvPlotAxis,
        mvHandlerRegistry, mvKeyDownHandler, mvKeyPressHandler,
        mvKeyReleaseHandler, mvMouseMoveHandler, mvMouseWheelHandler,
        mvMouseClickHandler, mvMouseDoubleClickHandler, mvMouseDownHandler,
        mvMouseReleaseHandler, mvMouseDragHandler, 
        mvHoverHandler, mvActiveHandler, mvFocusHandler, mvVisibleHandler,
        mvEditedHandler, mvActivatedHandler, mvDeactivatedHandler, mvDeactivatedAfterEditHandler,
        mvToggledOpenHandler, mvClickedHandler, mvDragPayload,
        ItemTypeCount
    };

    enum class StorageValueTypes
    {
        None = 0,
        Int, Int4,
        Float, Float4, FloatVect, 
        Double, Double4, DoubleVect,
        Series,
        Bool,
        String,
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
        MV_PARSER_ARG_LABEL         = 1 << 7,
        MV_PARSER_ARG_SOURCE        = 1 << 8,
        MV_PARSER_ARG_CALLBACK      = 1 << 9,
        MV_PARSER_ARG_USER_DATA     = 1 << 10,
        MV_PARSER_ARG_SHOW          = 1 << 11,
        MV_PARSER_ARG_ENABLED       = 1 << 12,
        MV_PARSER_ARG_POS           = 1 << 13,
        MV_PARSER_ARG_DROP_CALLBACK = 1 << 14,
        MV_PARSER_ARG_DRAG_CALLBACK = 1 << 15,
        MV_PARSER_ARG_PAYLOAD_TYPE  = 1 << 16,
        MV_PARSER_ARG_TRACKED       = 1 << 17,
        MV_PARSER_ARG_FILTER        = 1 << 18,
    };

    using mvValueVariant = std::variant<
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

        // items that need access to other items
        friend class mvMenuItem;
        friend class mvCollapsingHeader;
        friend class mvChild;
        friend class mvGroup;
        friend class mvTabBar;
        friend class mvTab;
        friend class mvMenuBar;
        friend class mvMenu;
        friend class mvWindow;
        friend class mvTooltip;
        friend class mvPopup;
        friend class mvTreeNode;
        friend class mvWindowAppItem;
        friend class mvTable;
        friend class mvNodeEditor;
        friend class mvNode;
        friend class mvNodeAttribute;
        friend class mvFontManager;
        friend class mvFontScope;
        friend class mvDrawlist;
        friend class mvDrawLayer;
        friend class mvPlot;
        friend class mvPlotAxis;
        friend class mvLineSeries;
        friend class mvAreaSeries;
        friend class mvBarSeries;
        friend class mvCandleSeries;
        friend class mvErrorSeries;
        friend class mvHeatSeries;
        friend class mvImageSeries;
        friend class mvVLineSeries;
        friend class mvHLineSeries;
        friend class mvLabelSeries;
        friend class mvPieSeries;
        friend class mvScatterSeries;
        friend class mvShadeSeries;
        friend class mvStairSeries;
        friend class mvStemSeries;
        friend class mvTextureContainer;
        friend class mvViewportDrawlist;
        friend class mvHistogramSeries;
        friend class mv2dHistogramSeries;
        friend class mvFilterSet;
        friend class mvClipper;
       
    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_CREATE_EXTRA_COMMAND(get_item_configuration);
        MV_CREATE_EXTRA_COMMAND(get_item_state);
        MV_CREATE_EXTRA_COMMAND(get_item_info);
        MV_CREATE_EXTRA_COMMAND(configure_item);
        MV_CREATE_EXTRA_COMMAND(get_value);
        MV_CREATE_EXTRA_COMMAND(get_values);
        MV_CREATE_EXTRA_COMMAND(set_value);
        MV_CREATE_EXTRA_COMMAND(focus_item);
        MV_CREATE_EXTRA_COMMAND(set_item_pos);
        MV_CREATE_EXTRA_COMMAND(reset_pos);

        MV_START_EXTRA_COMMANDS
            MV_ADD_EXTRA_COMMAND(get_item_configuration);
            MV_ADD_EXTRA_COMMAND(get_item_state);
            MV_ADD_EXTRA_COMMAND(get_item_info);
            MV_ADD_EXTRA_COMMAND(configure_item);
            MV_ADD_EXTRA_COMMAND(get_value);
            MV_ADD_EXTRA_COMMAND(get_values);
            MV_ADD_EXTRA_COMMAND(set_value);
            MV_ADD_EXTRA_COMMAND(focus_item);
            MV_ADD_EXTRA_COMMAND(set_item_pos);
            MV_ADD_EXTRA_COMMAND(reset_pos);
        MV_END_EXTRA_COMMANDS

        static bool DoesItemHaveFlag(mvAppItem* item, int flag);
        static std::pair<std::string, std::string> GetNameFromArgs(std::string& name, PyObject* args, PyObject* kwargs);
        static void AddCommonArgs(mvPythonParser& parser, CommonParserArgs args);

    protected:

            struct ScopedID
            {
                ScopedID() { ImGui::PushID(this); }
                ScopedID(void* id) { ImGui::PushID(id); }
                ~ScopedID() { ImGui::PopID(); }
            };

    public:

        mvAppItem(const std::string& name);
        mvAppItem(const mvAppItem& other) = delete; // copy constructor
        mvAppItem(mvAppItem&& other)      = delete; // move constructor

        virtual ~mvAppItem();

        //-----------------------------------------------------------------------------
        // These methods are overridden through the use of:
        //   - MV_REGISTER_WIDGET
        //   - MV_APPLY_WIDGET_REGISTRATION
        //-----------------------------------------------------------------------------
        [[nodiscard]] virtual mvAppItemType     getType      () const = 0;
        [[nodiscard]] virtual int               getDescFlags () const = 0;
        [[nodiscard]] virtual int               getTarget    () const = 0; // which child slot
        [[nodiscard]] virtual StorageValueTypes getValueType () const = 0;

        // actual immediate mode drawing instructions
        virtual bool preDraw();
        virtual void draw(ImDrawList* drawlist, float x, float y) = 0;
        virtual void postDraw();
        virtual void customAction() {};

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
        virtual bool           isParentCompatible(mvAppItemType type) { return true; }
        virtual bool           canChildBeAdded   (mvAppItemType type) { return true; }

    
        void                                getItemInfo(PyObject* dict);
        void                                checkArgs(PyObject* args, PyObject* kwargs);    
        void                                handleKeywordArgs(PyObject* dict);  // python dictionary acts as an out parameter 
        void                                getConfiguration(PyObject* dict);
        virtual void                        handleSpecificRequiredArgs(PyObject* args) {}
        virtual void                        handleSpecificPositionalArgs(PyObject* args) {}
        virtual void                        handleSpecificKeywordArgs(PyObject* dict) {}
        virtual void                        getSpecificConfiguration(PyObject* dict) {}

        //-----------------------------------------------------------------------------
        // These methods can be optionally overridden if your widget needs to be
        // notified when children are added/removed (i.e. tables, node editor)
        //-----------------------------------------------------------------------------
        virtual void                        onChildAdd    (mvRef<mvAppItem> item) {}
        virtual void                        onChildRemoved(mvRef<mvAppItem> item) {}
        virtual void                        onChildrenRemoved() {}

        void                                setPayloadType (const std::string& payloadType);
        void                                setCallback    (PyObject* callback);
        void                                setDragCallback(PyObject* callback);
        void                                setDropCallback(PyObject* callback);
        virtual void                        hide           () { m_show = false; }
        virtual void                        show           () { m_show = true; }
        void                                setCallbackData(PyObject* data);
        void                                updateLocations();

        std::vector<mvRef<mvAppItem>>&      getChildren(int slot);
        void                                setChildren(int slot, std::vector<mvRef<mvAppItem>> children);

        [[nodiscard]] bool                  isShown        () const { return m_show; }
        [[nodiscard]] PyObject*             getCallback    (bool ignore_enabled = true);  // returns the callback. If ignore_enable false and item is disabled then no callback will be returned.
        [[nodiscard]] PyObject*             getCallbackData()       { return m_user_data; }
        [[nodiscard]] PyObject*             getDragCallback()       { return m_dragCallback; }
        [[nodiscard]] PyObject*             getDropCallback()       { return m_dropCallback; }
        mvAppItemState&                     getState       () { return m_state; } 
        mvAppItem*                          getParent() { return m_parentPtr; }
        bool                                isEnabled() const { return m_enabled; }
        int                                 getWidth() const { return m_width; }
        int                                 getHeight() const { return m_height; }
        const std::string&                  getName() const { return m_name; }
        const std::string&                  getFilter() const { return m_filter; }
        mvAppItem*                          getRoot() const;
        int                                 getLocation() const { return m_location; }

        // theme colors
        mvThemeColorGroup&                        getColorGroup        ();
        mvThemeColorGroup&                        getDisabledColorGroup();
        void                                      inValidateThemeColorCache();
        void                                      inValidateThemeDisabledColorCache();

        // theme styles
        mvThemeStyleGroup&                        getStyleGroup();
        void                                      inValidateThemeStyleCache();
        bool                                      isThemeFontCacheValid() const;
        void                                      inValidateThemeFontCache();
        void                                      setThemeFontCacheValid();
        void                                      setFont(const std::string& name, int size, ImFont* font);
        ImFont*                                   getFont() { return m_cachefont; }
        ImFont*                                   getCachedFont() { return m_font; }
        const std::string&                        getFontName() const { return m_fontName; }
        int                                       getFontSize() const { return m_fontSize; }

    protected:

        virtual void                        setWidth                  (int width)               { m_width = width; }
        virtual void                        setHeight                 (int height)              { m_height = height; }
        virtual void                        setEnabled                (bool value)              { m_enabled = value; }
        virtual void                        setDataSource             (const std::string& value){ m_source = value; }
        virtual void                        setLabel                  (const std::string& value); 
        void                                setFilter                 (const std::string& value); 
        void                                setPos                    (const ImVec2& pos); 

    private:

        mvRef<mvAppItem>                    getChild(const std::string& name);      // will return nullptr if not found

        // runtime modifications
        bool                                addItem(mvRef<mvAppItem> item);
        bool                                addRuntimeChild(const std::string& parent, const std::string& before, mvRef<mvAppItem> item);
        bool                                addChildAfter(const std::string& prev, mvRef<mvAppItem> item);
        bool                                deleteChild(const std::string& name);
        void                                deleteChildren();
        bool                                moveChildUp(const std::string& name);
        bool                                moveChildDown(const std::string& name);
        void                                resetState();
        void                                registerWindowFocusing(); // only useful for imgui window types
        mvRef<mvAppItem>                    stealChild(const std::string& name); // steals a child (used for moving)

       
    protected:

        mvAppItemState                m_state;
        bool                          m_focusNextFrame = false;
        bool                          m_dirtyPos = false;
        ImVec2                        m_previousCursorPos = { 0.0f, 0.0f };
        int                           m_location = -1;

        mvAppItem*                    m_parentPtr = nullptr;
        std::vector<mvRef<mvAppItem>> m_children[5] = { {}, {}, {}, {}, {} };

        std::string                   m_label; // internal label

        // new themes
        mvThemeColorGroup m_colors         = mvThemeColorGroup(this);
        mvThemeColorGroup m_disabledColors = mvThemeColorGroup(this);
        mvThemeStyleGroup m_styles         = mvThemeStyleGroup(this);
        bool              m_theme_font_dirty = false;

        // fonts
        ImFont* m_font = nullptr;
        ImFont* m_cachefont = nullptr;
        std::string m_fontName = "";
        int m_fontSize = 0;

        // config
        std::string    m_name = "";
        std::string    m_source = "";
        std::string    m_specificedlabel = "__DearPyGuiDefault";
        std::string    m_parent = "";
        std::string    m_before = "";
        std::string    m_filter = "";
        int            m_width = 0;
        int            m_height = 0;
        float          m_indent = -1.0f;
        int            m_windowPosx = 0;
        int            m_windowPosy = 0;
        int            m_posx = 0;
        int            m_posy = 0;
        bool           m_show = true;
        bool           m_enabled = true;
        PyObject*      m_callback = nullptr;
        PyObject*      m_user_data = nullptr;
        bool           m_tracked = false;
        float          m_trackOffset = 0.5f; // 0.0f:top, 0.5f:center, 1.0f:bottom

        // drag & drop
        PyObject* m_dragCallback = nullptr;
        PyObject* m_dropCallback = nullptr;
        std::string m_payloadType = "$$DPG_PAYLOAD";

    };

    inline void DecodeType(long encoded_constant, mvAppItemType* type) { *type = (mvAppItemType)(encoded_constant / 1000); }

}