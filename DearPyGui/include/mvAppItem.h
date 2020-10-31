#pragma once

//-----------------------------------------------------------------------------
// mvAppItem
//
//     - mvAppItem is the abstract base class for all DearPygui widgets.
//     
//-----------------------------------------------------------------------------

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <string>
#include <vector>
#include <map>
#include <imgui.h>
#include "mvCore.h"
#include "mvAppItemState.h"

//-----------------------------------------------------------------------------
// Helper Macro
//-----------------------------------------------------------------------------
#define MV_APPITEM_TYPE(x, parser)\
    mvAppItemType getType() const override { return x; }\
    std::string getStringType() const override { return std::string(#x); }\
    std::string getParserCommand() const override { return parser; }

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
        StyleWindow, FileDialog
    };

    //-----------------------------------------------------------------------------
    // mvAppItem
    //-----------------------------------------------------------------------------
    class mvAppItem
    {

        struct StyleColor
        {
            ImGuiCol idx;
            mvColor color;
        };

    public:

        mvAppItem(const std::string& name);

        virtual ~mvAppItem();

        mvAppItem          (const mvAppItem& other) = delete; // copy constructor
        mvAppItem          (mvAppItem&& other)      = delete; // move constructor

        // pure virtual methods
        [[nodiscard]] virtual mvAppItemType getType      () const = 0;
        [[nodiscard]] virtual std::string   getStringType() const = 0;
        virtual void                        draw         ()       = 0; // actual imgui draw commands

        // virtual methods
        virtual std::string                 getParserCommand     () const { return "no_command_set"; }
        virtual void                        updateData           (const std::string& name) {}
        [[nodiscard]] virtual bool          areDuplicatesAllowed () const { return false; }
        [[nodiscard]] virtual bool          canBeDeleted         () const { return true; }
        [[nodiscard]] virtual bool          isARoot              () const { return false; }

        // configuration get/set
        void                                checkConfigDict   (PyObject* dict);
        void                                setConfigDict     (PyObject* dict);  // python dictionary acts as an out parameter 
        void                                getConfigDict     (PyObject* dict);
        virtual void                        setExtraConfigDict(PyObject* dict) {}
        virtual void                        getExtraConfigDict(PyObject* dict) {}

        // color styles for runtime
        void                                addColorStyle  (ImGuiCol item, mvColor color);
        void                                pushColorStyles();
        void                                popColorStyles ();
        void                                clearColors    ();

        // runtime modifications
        bool                                addRuntimeChild       (const std::string& parent, const std::string& before, mvAppItem* item);
        bool                                addChildAfter         (const std::string& prev, mvAppItem* item);
        bool                                deleteChild           (const std::string& name);
        void                                deleteChildren        ();
        bool                                moveChildUp           (const std::string& name);
        bool                                moveChildDown         (const std::string& name);
        void                                resetState            ();
        void                                updateDataSource      (const std::string& name);
        void                                registerWindowFocusing(); // only useful for imgui window types
        mvAppItem*                          stealChild            (const std::string& name); // steals a child (used for moving)

        // getters
        mvAppItem*                          getChild                  (const std::string& name);      // will return nullptr if not found
        inline std::vector<mvAppItem*>&     getChildren               ()       { return m_children; }
        inline mvAppItem*                   getParent                 ()       { return m_parent; }   // can return nullptr
        [[nodiscard]] const std::string&    getName                   () const { return m_name; }
        [[nodiscard]] const std::string&    getTip                    () const { return m_tip; }
        [[nodiscard]] const std::string&    getLabel                  () const { return m_label; }
        [[nodiscard]] PyObject*             getCallback               (bool ignore_enabled = true);  // returns the callback. If ignore_enable false and item is disabled then no callback will be returned.
        [[nodiscard]] PyObject*             getCallbackData           ()       { return m_callbackData; }
        [[nodiscard]] const std::string&    getPopup                  () const { return m_popup; }
        [[nodiscard]] const std::string&    getDataSource             () const { return m_dataSource; }
        [[nodiscard]] int                   getWidth                  () const { return m_width; }
        [[nodiscard]] int                   getHeight                 () const { return m_height; }
        [[nodiscard]] bool                  isShown                   () const { return m_show; }
        [[nodiscard]] bool                  isContainer               () const { return m_container; }
        [[nodiscard]] bool                  isItemEnabled             () const { return m_enabled; }
        mvAppItemState&                     getState                  ()       { return m_state; }

        // setters
        void                                setParent                 (mvAppItem* parent);
        void                                showAll                   ();
        void                                hideAll                   ();
        void                                addChild                  (mvAppItem* child);
        inline void                         show                      ()                        { m_show = true; }
        inline void                         hide                      ()                        { m_show = false; }
        void                                setCallback               (PyObject* callback);
        inline void                         setCallbackData           (PyObject* data)          { m_callbackData = data; }
        inline void                         setPopup                  (const std::string& popup){ m_popup = popup; }
        inline void                         setTip                    (const std::string& tip)  { m_tip = tip; }
        virtual void                        setWidth                  (int width)               { m_width = width; }
        virtual void                        setHeight                 (int height)              { m_height = height; }
        virtual void                        setEnabled                (bool value)              { m_enabled = value; }
        virtual void                        setDataSource             (const std::string& value){ m_dataSource = value; }
        inline void                         setLabel                  (const std::string& value){ m_label = value; }

    protected:

        mvAppItemState          m_state;
        std::string             m_dataSource;
        std::string             m_name;
        std::string             m_label;
        std::string             m_popup;
        std::string             m_tip;
        PyObject*               m_callback = nullptr;
        PyObject*               m_callbackData = nullptr;
        int                     m_width  = 0;
        int                     m_height = 0;
        bool                    m_show                 = true; // determines whether to attempt rendering
        bool                    m_container            = false;
        bool                    m_enabled              = true;
        mvAppItem*              m_parent               = nullptr;
        std::vector<mvAppItem*> m_children;
        std::vector<StyleColor> m_colors;
    };
}