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
#include "mvCore.h"
#include "mvAppItemState.h"
#include "mvAppItemDescription.h"
#include "mvAppItemStyleManager.h"
#include "mvCallbackRegistry.h"
#include "mvTheme.h"

//-----------------------------------------------------------------------------
// Helper Macros
//-----------------------------------------------------------------------------
#include "cpp.hint"

namespace Marvel {

    //-----------------------------------------------------------------------------
    // Widget Types
    //-----------------------------------------------------------------------------
    enum class mvAppItemType
    {
        None = 0, Spacing, SameLine, InputText, Button, 
        RadioButtons, TabBar, TabItem, Image, MenuBar, 
        Menu, EndMenu, MenuItem, EndMenuBar, Group, Child, 
        SliderFloat, SliderFloat4, SliderInt, SliderInt4, 
        SliderFloat2, SliderFloat3, SliderInt2, SliderInt3,
        DragFloat, DragFloat4, DragInt, DragInt4, DragFloat2,
        DragFloat3, DragInt2, DragInt3, InputFloat, InputFloat4, 
        InputInt, InputInt4, InputFloat2, InputFloat3, 
        InputInt2, InputInt3, ColorEdit3, ColorEdit4, 
        ColorPicker3, ColorPicker4, Tooltip, CollapsingHeader, 
        Separator, Checkbox, Listbox, Text, LabelText, Combo, 
        Plot, SimplePlot, Indent, Unindent, Drawing, Window,
        Popup, Selectable, TreeNode, ProgressBar, Table, Dummy,
        ImageButton, TimePicker, DatePicker, ColorButton,
        ManagedColumns, ColumnSet, NextColumn, Logger,
        AboutWindow, DocWindow, DebugWindow, MetricsWindow,
        StyleWindow, FileDialog, TabButton, 
        ItemTypeCount
    };

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
        friend class mvManagedColumns;

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
        virtual std::string                 getParserCommand     () const { return "no_command_set"; }

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
        mvAppItemStyleManager&              getStyleManager() { return m_styleManager; }
        mvAppItem*                          getParent() { return m_parent; }

        // theme get/set
        std::unordered_map<mvAppItemType, ThemeColors>& getColors() { return m_colors; }
        virtual const std::vector<std::pair<std::string, long>>& getColorConstants() const 
        { 
            static std::vector<std::pair<std::string, long>> constants;
            return constants;
        }
        std::unordered_map<mvAppItemType, ThemeStyles>& getStyles() { return m_styles; }

        // cpp interface
        virtual void updateConfig(mvAppItemConfig* config) {}
        virtual mvAppItemConfig* getConfig() { return nullptr; }
        void updateCoreConfig();
        mvAppItemConfig& getCoreConfig();

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
        mvAppItemStyleManager         m_styleManager;
        mvAppItemDescription          m_description;
        mvAppItemConfig               m_core_config;

        mvAppItem*                    m_parent = nullptr;
        std::vector<mvRef<mvAppItem>> m_children;

        std::string                   m_label; // internal label

        std::unordered_map<mvAppItemType, ThemeColors> m_colors;
        std::unordered_map<mvAppItemType, ThemeStyles> m_styles;
    };

}