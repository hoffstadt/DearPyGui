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
        mvSliderFloat, mvSliderInt,
        mvDragFloat, mvDragInt, mvInputFloat,
        mvInputInt, mvColorEdit,
        mvColorPicker, mvTooltip, mvCollapsingHeader,
        mvSeparator, mvCheckbox, mvListbox, mvText, mvLabelText, mvCombo,
        mvPlot, mvSimplePlot, mvDrawing, mvWindowAppItem,
        mvPopup, mvSelectable, mvTreeNode, mvProgressBar, mvDataGrid, mvDummy,
        mvImageButton, mvTimePicker, mvDatePicker, mvColorButton,
        mvAboutWindow, mvDocWindow, mvDebugWindow, mvMetricsWindow,
        mvStyleWindow, mvFileDialog, mvTabButton, mvLoggerItem,
        mvNodeEditor, mvNode, mvNodeAttribute,
        mvTable, mvTableColumn, mvTableNextColumn,
        mvDrawLine, mvDrawArrow, mvDrawTriangle, mvDrawCircle, mvDrawBezierCurve,
        mvDrawQuad, mvDrawRect, mvDrawText, mvDrawPolygon, mvDrawPolyline,
        mvDrawImage, mvDragFloatMulti, mvDragIntMulti, mvSliderFloatMulti,
        mvSliderIntMulti, mvInputIntMulti, mvInputFloatMulti,
        mvDragPoint, mvDragLine, mvAnnotation, mvLineSeries,
        mvScatterSeries, mvStemSeries, mvStairSeries, mvBarSeries,
        mvErrorSeries, mvVLineSeries, mvHLineSeries, mvHeatSeries,
        mvImageSeries, mvPieSeries, mvShadeSeries, mvLabelSeries,
        mvCandleSeries, mvAreaSeries, mvColorMapScale,
        ItemTypeCount
    };

        enum class StorageValueTypes
    {
        None = 0,
        Int, Int4,
        Float, Float4, FloatVect, 
        Series,
        Bool,
        String,
        Time, Color
    };

    enum ItemDescriptionFlags
    {
        MV_ITEM_DESC_DEFAULT     = 0,
        MV_ITEM_DESC_ROOT        = 1 << 1,
        MV_ITEM_DESC_CONTAINER   = 1 << 2,
        MV_ITEM_DESC_AFTER       = 1 << 3,
    };

    using mvValueVariant = std::variant<
        std::shared_ptr<int>,
        std::shared_ptr<std::array<int, 4>>,
        std::shared_ptr<float >,
        std::shared_ptr<std::array<float, 4>>,
        std::shared_ptr<std::vector<float>>,
        std::shared_ptr<std::vector<std::vector<float>>>,
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
        friend class mvDebugWindow;

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
        friend class mvDrawing;
        friend class mvPlot;

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_CREATE_EXTRA_COMMAND(get_item_configuration);
        MV_CREATE_EXTRA_COMMAND(configure_item);
        MV_CREATE_EXTRA_COMMAND(get_item_type);
        MV_CREATE_EXTRA_COMMAND(get_value);
        MV_CREATE_EXTRA_COMMAND(set_value);
        MV_CREATE_EXTRA_COMMAND(is_item_hovered);
        MV_CREATE_EXTRA_COMMAND(is_item_shown);
        MV_CREATE_EXTRA_COMMAND(is_item_active);
        MV_CREATE_EXTRA_COMMAND(is_item_focused);
        MV_CREATE_EXTRA_COMMAND(is_item_clicked);
        MV_CREATE_EXTRA_COMMAND(is_item_container);
        MV_CREATE_EXTRA_COMMAND(is_item_visible);
        MV_CREATE_EXTRA_COMMAND(is_item_edited);
        MV_CREATE_EXTRA_COMMAND(is_item_activated);
        MV_CREATE_EXTRA_COMMAND(is_item_deactivated);
        MV_CREATE_EXTRA_COMMAND(is_item_deactivated_after_edit);
        MV_CREATE_EXTRA_COMMAND(is_item_toggled_open);
        MV_CREATE_EXTRA_COMMAND(get_item_rect_min);
        MV_CREATE_EXTRA_COMMAND(get_item_rect_max);
        MV_CREATE_EXTRA_COMMAND(get_item_rect_size);
        MV_CREATE_EXTRA_COMMAND(get_item_parent);
        MV_CREATE_EXTRA_COMMAND(get_item_children);

        MV_START_EXTRA_COMMANDS
            MV_ADD_EXTRA_COMMAND(get_item_configuration);
            MV_ADD_EXTRA_COMMAND(configure_item);
            MV_ADD_EXTRA_COMMAND(get_item_type);
            MV_ADD_EXTRA_COMMAND(get_value);
            MV_ADD_EXTRA_COMMAND(set_value);
            MV_ADD_EXTRA_COMMAND(is_item_hovered);
            MV_ADD_EXTRA_COMMAND(is_item_shown);
            MV_ADD_EXTRA_COMMAND(is_item_active);
            MV_ADD_EXTRA_COMMAND(is_item_focused);
            MV_ADD_EXTRA_COMMAND(is_item_clicked);
            MV_ADD_EXTRA_COMMAND(is_item_container);
            MV_ADD_EXTRA_COMMAND(is_item_visible);
            MV_ADD_EXTRA_COMMAND(is_item_edited);
            MV_ADD_EXTRA_COMMAND(is_item_activated);
            MV_ADD_EXTRA_COMMAND(is_item_deactivated);
            MV_ADD_EXTRA_COMMAND(is_item_deactivated_after_edit);
            MV_ADD_EXTRA_COMMAND(is_item_toggled_open);
            MV_ADD_EXTRA_COMMAND(get_item_rect_min);
            MV_ADD_EXTRA_COMMAND(get_item_rect_max);
            MV_ADD_EXTRA_COMMAND(get_item_rect_size);
            MV_ADD_EXTRA_COMMAND(get_item_parent);
            MV_ADD_EXTRA_COMMAND(get_item_children);
        MV_END_EXTRA_COMMANDS

        static bool DoesItemHaveFlag(mvAppItem* item, int flag);
        static std::pair<std::string, std::string> GetNameFromArgs(std::string& name, PyObject* args, PyObject* kwargs);
        static void AddCommonArgs(mvPythonParser& parser);

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
        virtual void draw(ImDrawList* drawlist, float x, float y) = 0;

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

    
        void                                checkConfigDict(PyObject* dict);    
        void                                setConfigDict(PyObject* dict);  // python dictionary acts as an out parameter 
        void                                getConfigDict(PyObject* dict);
        virtual void                        setConfigArgs(PyObject* args) {}
        virtual void                        setExtraConfigDict(PyObject* dict) {}
        virtual void                        getExtraConfigDict(PyObject* dict) {}

        //-----------------------------------------------------------------------------
        // These methods can be optionally overridden if your widget needs to be
        // notified when children are added/removed (i.e. tables, node editor)
        //-----------------------------------------------------------------------------
        virtual void                        onChildAdd    (mvRef<mvAppItem> item) {}
        virtual void                        onChildRemoved(mvRef<mvAppItem> item) {}
        virtual void                        onChildrenRemoved() {}

        void                                setCallback    (PyObject* callback);
        void                                hide           () { m_show = false; }
        void                                show           () { m_show = true; }
        void                                setCallbackData(PyObject* data);

        [[nodiscard]] bool                  isShown        () const { return m_show; }
        [[nodiscard]] PyObject*             getCallback    (bool ignore_enabled = true);  // returns the callback. If ignore_enable false and item is disabled then no callback will be returned.
        [[nodiscard]] PyObject*             getCallbackData()       { return m_callback_data; }
        mvAppItemState&                     getState       () { return m_state; } 
        mvAppItem*                          getParent() { return m_parentPtr; }
        bool                                isEnabled() const { return m_enabled; }

        // theme get/set
        std::unordered_map<mvAppItemType, mvThemeColors>& getColors() { return m_colors; }
        std::unordered_map<mvAppItemType, mvThemeStyles>& getStyles() { return m_styles; }

        // cached theming
        bool                                      isThemeColorCacheValid() const;
        bool                                      isThemeStyleCacheValid() const;
        bool                                      isThemeFontCacheValid() const;
        void                                      inValidateThemeColorCache();
        void                                      inValidateThemeStyleCache();
        void                                      inValidateThemeFontCache();
        void                                      setThemeColorCacheValid();
        void                                      setThemeStyleCacheValid();
        void                                      setThemeFontCacheValid();
        void                                      setFont(ImFont* font) { m_cached_font = font; }
        ImFont*                                   getCachedFont();
        mvThemeColors&                            getCachedThemeColors();
        std::unordered_map<ImGuiStyleVar, float>& getCachedThemeStyles();
        std::unordered_map<ImGuiStyleVar, float>& getCachedThemeStyles1();
        std::unordered_map<ImGuiStyleVar, float>& getCachedThemeStyles2();

    protected:

        virtual void                        setWidth                  (int width)               { m_width = width; }
        virtual void                        setHeight                 (int height)              { m_height = height; }
        virtual void                        setEnabled                (bool value)              { m_enabled = value; }
        virtual void                        setDataSource             (const std::string& value){ m_source = value; }
        virtual void                        setLabel                  (const std::string& value); 

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

        mvAppItem*                    m_parentPtr = nullptr;
        std::vector<mvRef<mvAppItem>> m_children[2] = { {}, {} };

        std::string                   m_label; // internal label

        std::unordered_map<mvAppItemType, mvThemeColors> m_colors;
        std::unordered_map<mvAppItemType, mvThemeStyles> m_styles;

        // cached theming
        bool                                     m_theme_color_dirty = true;
        bool                                     m_theme_style_dirty = true;
        bool                                     m_theme_font_dirty = false;
        mvThemeColors                            m_cached_colors;
        std::unordered_map<ImGuiStyleVar, float> m_cached_styles;
        std::unordered_map<ImGuiStyleVar, float> m_cached_styles1;
        std::unordered_map<ImGuiStyleVar, float> m_cached_styles2;

        // fonts
        ImFont* m_cached_font = nullptr;

        // config
        std::string    m_name = "";
        std::string    m_source = "";
        std::string    m_specificedlabel = "__DearPyGuiDefault";
        std::string    m_parent = "";
        std::string    m_before = "";
        int            m_width = 0;
        int            m_height = 0;
        int            m_windowPosx = 0;
        int            m_windowPosy = 0;
        int            m_posx = 0;
        int            m_posy = 0;
        bool           m_show = true;
        bool           m_enabled = true;
        PyObject*      m_callback = nullptr;
        PyObject*      m_callback_data = nullptr;

    };

    inline void DecodeType(long encoded_constant, mvAppItemType* type) { *type = (mvAppItemType)(encoded_constant / 1000); }

}