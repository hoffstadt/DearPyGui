#pragma once

//-----------------------------------------------------------------------------
// mvAppItem
//
//     - mvAppItem is the abstract base class for all DearPygui widgets.
//     
//-----------------------------------------------------------------------------

#include "mvPython.h"
#include <string>
#include <vector>
#include <map>
#include <imgui.h>
#include "mvAppItemState.h"
#include "mvAppItemDescription.h"
#include "mvCallbackRegistry.h"

//-----------------------------------------------------------------------------
// Helper Macros
//-----------------------------------------------------------------------------
#include "cpp.hint"

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
        mvSliderFloat, mvSliderFloat4, mvSliderInt, mvSliderInt4,
        mvSliderFloat2, mvSliderFloat3, mvSliderInt2, mvSliderInt3,
        mvDragFloat, mvDragFloat4, mvDragInt, mvDragInt4, mvDragFloat2,
        mvDragFloat3, mvDragInt2, mvDragInt3, mvInputFloat, mvInputFloat4,
        mvInputInt, mvInputInt4, mvInputFloat2, mvInputFloat3,
        mvInputInt2, mvInputInt3, mvColorEdit3, mvColorEdit4,
        mvColorPicker3, mvColorPicker4, mvTooltip, mvCollapsingHeader,
        mvSeparator, mvCheckbox, mvListbox, mvText, mvLabelText, mvCombo,
        mvPlot, mvSimplePlot, mvIndent, mvUnindent, mvDrawing, mvWindowAppItem,
        mvPopup, mvSelectable, mvTreeNode, mvProgressBar, mvDataGrid, mvDummy,
        mvImageButton, mvTimePicker, mvDatePicker, mvColorButton,
        mvAboutWindow, mvDocWindow, mvDebugWindow, mvMetricsWindow,
        mvStyleWindow, mvFileDialog, mvTabButton, mvLoggerItem,
        mvNodeEditor, mvNode, mvNodeAttribute,
        mvTable, mvTableColumn, mvTableNextColumn,
        mvDrawLine, mvDrawArrow, mvDrawTriangle, mvDrawCircle, mvDrawBezierCurve,
        mvDrawQuad, mvDrawRect, mvDrawText, mvDrawPolygon, mvDrawPolyline,
        mvDrawImage,
        ItemTypeCount
    };

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

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_CREATE_EXTRA_COMMAND(get_item_configuration);
        MV_CREATE_EXTRA_COMMAND(configure_item);
        MV_CREATE_EXTRA_COMMAND(get_item_type);
        MV_CREATE_EXTRA_COMMAND(set_item_callback);
        MV_CREATE_EXTRA_COMMAND(set_item_callback_data);
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
        MV_CREATE_EXTRA_COMMAND(get_item_callback);
        MV_CREATE_EXTRA_COMMAND(get_item_callback_data);
        MV_CREATE_EXTRA_COMMAND(get_item_parent);
        MV_CREATE_EXTRA_COMMAND(get_item_children);

        MV_START_EXTRA_COMMANDS
            MV_ADD_EXTRA_COMMAND(get_item_configuration);
            MV_ADD_EXTRA_COMMAND(configure_item);
            MV_ADD_EXTRA_COMMAND(get_item_type);
            MV_ADD_EXTRA_COMMAND(set_item_callback);
            MV_ADD_EXTRA_COMMAND(set_item_callback_data);
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
            MV_ADD_EXTRA_COMMAND(get_item_callback);
            MV_ADD_EXTRA_COMMAND(get_item_callback_data);
            MV_ADD_EXTRA_COMMAND(get_item_parent);
            MV_ADD_EXTRA_COMMAND(get_item_children);
        MV_END_EXTRA_COMMANDS

    protected:

            struct ScopedID
            {
                ScopedID() { ImGui::PushID(this); }
                ScopedID(void* id) { ImGui::PushID(id); }
                ~ScopedID() { ImGui::PopID(); }
            };

    public:

        mvAppItem(const std::string& name);

        virtual ~mvAppItem();

        mvAppItem(const mvAppItem& other) = delete; // copy constructor
        mvAppItem(mvAppItem&& other)      = delete; // move constructor

        // pure virtual methods
        [[nodiscard]] virtual mvAppItemType getType      () const = 0;
        [[nodiscard]] virtual std::string   getStringType() const = 0;
        virtual void                        draw         (ImDrawList* drawlist, float x, float y)       = 0; // actual imgui draw commands

        // virtual methods
        virtual std::string    getParserCommand     () const { return "no_command_set"; }
        virtual mvValueVariant getValue() { return nullptr; }
        virtual PyObject*      getPyValue() { return GetPyNone(); }
        virtual void           setPyValue(PyObject* value) { }

        // registy helpers
        virtual bool           isParentCompatible(mvAppItemType type) { return true; }
        virtual bool           canChildBeAdded   (mvAppItemType type) { return true; }

        // configuration get/set
        void                                checkConfigDict(PyObject* dict);
        void                                setConfigDict(PyObject* dict);  // python dictionary acts as an out parameter 
        void                                getConfigDict(PyObject* dict);
        virtual void                        setExtraConfigDict(PyObject* dict) {}
        virtual void                        getExtraConfigDict(PyObject* dict) {}

        void                                setCallback    (mvCallable callback);
        void                                hide           () { m_show = false; }
        void                                show           () { m_show = true; }
        void                                setCallbackData(mvCallableData data);

        [[nodiscard]] bool                  isShown        () const { return m_show; }
        [[nodiscard]] mvCallable            getCallback    (bool ignore_enabled = true);  // returns the callback. If ignore_enable false and item is disabled then no callback will be returned.
        [[nodiscard]] mvCallableData        getCallbackData()       { return m_callback_data; }
        const mvAppItemDescription&         getDescription () const { return m_description; }
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
        mvAppItemDescription          m_description;

        mvAppItem*                    m_parentPtr = nullptr;
        std::vector<mvRef<mvAppItem>> m_children0;
        std::vector<mvRef<mvAppItem>> m_children1;

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
        mvCallable     m_callback = nullptr;
        mvCallableData m_callback_data = nullptr;

    };

    inline void DecodeType(long encoded_constant, mvAppItemType* type) { *type = (mvAppItemType)(encoded_constant / 1000); }

}