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
        ItemTypeCount
    };

    template<int item_type> struct mvItemType {};

    //-----------------------------------------------------------------------------
    // Core Config Struct
    //-----------------------------------------------------------------------------
    struct mvAppItemConfig
    {
        std::string name = "";
        std::string source = "";
        std::string label = "__DearPyGuiDefault";
        std::string parent = "";
        std::string before = "";
        int width = 0;
        int height = 0;
        bool show = true;
        bool enabled = true;
        mvCallable callback = nullptr;
        mvCallableData callback_data = nullptr;
        
    };

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

    protected:

            struct ScopedID
            {
                ScopedID() { ImGui::PushID(this); }
                ScopedID(void* id) { ImGui::PushID(id); }
                ~ScopedID() { ImGui::PopID(); }
            };

    public:

        mvAppItem(const std::string& name);
        mvAppItem(const mvAppItemConfig& config);

        virtual ~mvAppItem();

        mvAppItem(const mvAppItem& other) = delete; // copy constructor
        mvAppItem(mvAppItem&& other)      = delete; // move constructor

        // pure virtual methods
        [[nodiscard]] virtual mvAppItemType getType      () const = 0;
        [[nodiscard]] virtual std::string   getStringType() const = 0;
        virtual void                        draw         ()       = 0; // actual imgui draw commands

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
        void                                hide           () { m_core_config.show = false; }
        void                                show           () { m_core_config.show = true; }
        void                                setCallbackData(mvCallableData data);

        [[nodiscard]] bool                  isShown        () const { return m_core_config.show; }
        [[nodiscard]] mvCallable            getCallback    (bool ignore_enabled = true);  // returns the callback. If ignore_enable false and item is disabled then no callback will be returned.
        [[nodiscard]] mvCallableData        getCallbackData()       { return m_core_config.callback_data; }
        const mvAppItemDescription&         getDescription () const { return m_description; }
        mvAppItemState&                     getState       () { return m_state; } 
        mvAppItem*                          getParent() { return m_parent; }

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


        //-----------------------------------------------------------------------------
        // cpp interface
        //-----------------------------------------------------------------------------
        virtual void             updateConfig    (mvAppItemConfig* config) {}
        virtual mvAppItemConfig* getConfig       () { return nullptr; }
        void                     updateCoreConfig();
        mvAppItemConfig&         getCoreConfig   ();

    protected:

        virtual void                        setWidth                  (int width)               { m_core_config.width = width; }
        virtual void                        setHeight                 (int height)              { m_core_config.height = height; }
        virtual void                        setEnabled                (bool value)              { m_core_config.enabled = value; }
        virtual void                        setDataSource             (const std::string& value){ m_core_config.source = value; }
        virtual void                        setLabel                  (const std::string& value); 

    private:

        mvRef<mvAppItem>                    getChild(const std::string& name);      // will return nullptr if not found

        // runtime modifications
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
        mvAppItemConfig               m_core_config;

        mvAppItem*                    m_parent = nullptr;
        std::vector<mvRef<mvAppItem>> m_children;

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

    };

    inline void DecodeType(long encoded_constant, mvAppItemType* type) { *type = (mvAppItemType)(encoded_constant / 1000); }

    void AddItemCommands(std::map<std::string, mvPythonParser>* parsers);

    PyObject* get_item_type(PyObject* self, PyObject* args, PyObject* kwargs);

    PyObject* get_item_configuration(PyObject* self, PyObject* args, PyObject* kwargs);
    PyObject* configure_item(PyObject* self, PyObject* args, PyObject* kwargs);

    // replacing
    PyObject* set_item_callback(PyObject* self, PyObject* args, PyObject* kwargs);
    PyObject* set_item_callback_data(PyObject* self, PyObject* args, PyObject* kwargs);

    PyObject* move_item(PyObject* self, PyObject* args, PyObject* kwargs);
    PyObject* delete_item(PyObject* self, PyObject* args, PyObject* kwargs);
    PyObject* does_item_exist(PyObject* self, PyObject* args, PyObject* kwargs);
    PyObject* move_item_up(PyObject* self, PyObject* args, PyObject* kwargs);
    PyObject* move_item_down(PyObject* self, PyObject* args, PyObject* kwargs);
    PyObject* get_item_callback(PyObject* self, PyObject* args, PyObject* kwargs);
    PyObject* get_item_callback_data(PyObject* self, PyObject* args, PyObject* kwargs);

    PyObject* get_item_children(PyObject* self, PyObject* args, PyObject* kwargs);
    PyObject* get_all_items(PyObject* self, PyObject* args, PyObject* kwargs);
    PyObject* get_windows(PyObject* self, PyObject* args, PyObject* kwargs);

    PyObject* get_item_parent(PyObject* self, PyObject* args, PyObject* kwargs);

    PyObject* is_item_hovered(PyObject* self, PyObject* args, PyObject* kwargs);
    PyObject* is_item_shown(PyObject* self, PyObject* args, PyObject* kwargs);
    PyObject* is_item_active(PyObject* self, PyObject* args, PyObject* kwargs);
    PyObject* is_item_focused(PyObject* self, PyObject* args, PyObject* kwargs);
    PyObject* is_item_clicked(PyObject* self, PyObject* args, PyObject* kwargs);
    PyObject* is_item_container(PyObject* self, PyObject* args, PyObject* kwargs);
    PyObject* is_item_visible(PyObject* self, PyObject* args, PyObject* kwargs);
    PyObject* is_item_edited(PyObject* self, PyObject* args, PyObject* kwargs);
    PyObject* is_item_activated(PyObject* self, PyObject* args, PyObject* kwargs);
    PyObject* is_item_deactivated(PyObject* self, PyObject* args, PyObject* kwargs);
    PyObject* is_item_deactivated_after_edit(PyObject* self, PyObject* args, PyObject* kwargs);
    PyObject* is_item_toggled_open(PyObject* self, PyObject* args, PyObject* kwargs);
    PyObject* get_item_rect_min(PyObject* self, PyObject* args, PyObject* kwargs);
    PyObject* get_item_rect_max(PyObject* self, PyObject* args, PyObject* kwargs);
    PyObject* get_item_rect_size(PyObject* self, PyObject* args, PyObject* kwargs);
    PyObject* get_value(PyObject* self, PyObject* args, PyObject* kwargs);
    PyObject* set_value(PyObject* self, PyObject* args, PyObject* kwargs);

}