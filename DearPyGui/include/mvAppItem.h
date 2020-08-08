#pragma once

//-----------------------------------------------------------------------------
// mvAppItem
//
//     - mvAppItem is the abstract base class for all MarvelSandbox widgets.
//     
//-----------------------------------------------------------------------------

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <string>
#include <vector>
#include <map>
#include <imgui.h>
#include "mvCore.h"

//-----------------------------------------------------------------------------
// Helper Macro
//-----------------------------------------------------------------------------
#define MV_APPITEM_TYPE(x) virtual mvAppItemType getType() const override { return x; } virtual std::string getStringType() const override { return std::string(#x); }

namespace Marvel {

	//-----------------------------------------------------------------------------
	// Enums
	//-----------------------------------------------------------------------------
	enum class mvAppItemType
	{
		None = 0, Spacing, SameLine,
		InputText, Button, RadioButtons,
		TabBar, TabItem, Image,
		MenuBar, Menu, EndMenu, MenuItem, EndMenuBar,
		Group, Child, 
		SliderFloat, SliderFloat4, SliderInt, SliderInt4, SliderFloat2, SliderFloat3, SliderInt2, SliderInt3,
		DragFloat, DragFloat4, DragInt, DragInt4, DragFloat2, DragFloat3, DragInt2, DragInt3,
		InputFloat, InputFloat4, InputInt, InputInt4, InputFloat2, InputFloat3, InputInt2, InputInt3,
		ColorEdit3, ColorEdit4, ColorPicker3, ColorPicker4,
		Tooltip, CollapsingHeader, Separator, Checkbox,
		Listbox, Text, LabelText, Combo, Plot, SimplePlot,
		Indent, Unindent, Drawing, Window,
		Popup, Selectable, TreeNode, ProgressBar,
		Table
	};

	//-----------------------------------------------------------------------------
	// mvAppItem
	//-----------------------------------------------------------------------------
	class mvAppItem
	{

	public:

		mvAppItem(const std::string& parent, const std::string& name);

		virtual ~mvAppItem();

		mvAppItem          (const mvAppItem& other) = delete; // copy constructor
		mvAppItem          (mvAppItem&& other)      = delete; // move constructor
		//mvAppItem operator=(const mvAppItem& other) = delete; // copy assignment operator
		//mvAppItem operator=(mvAppItem&& other)      = delete; // move assignment operator

		// pure virtual methods
		[[nodiscard]] virtual mvAppItemType getType      () const = 0;
		[[nodiscard]] virtual std::string   getStringType() const = 0;
		virtual void                        draw         ()       = 0;

		// virtual methods
		virtual void                    setPyValue          (PyObject* value) {}
        virtual void                    updateData          (const std::string& name) {}
		[[nodiscard]] virtual PyObject* getPyValue          () const { Py_RETURN_NONE; }
		[[nodiscard]] virtual bool      areDuplicatesAllowed() const { return false; }

		// color styles for runtime (WORK NEEDED HERE)
		void addColorStyle  (ImGuiCol item, mvColor color);
		void pushColorStyles();
		void popColorStyles ();

		// runtime modifications
		bool addRuntimeChild       (const std::string& parent, const std::string& before, mvAppItem* item);
		bool deleteChild           (const std::string& name);
		void deleteChildren        ();
		bool moveChildUp           (const std::string& name);
		bool moveChildDown         (const std::string& name);
		void resetState            ();
		void updateDataSource      (const std::string& name);
		void registerWindowFocusing(); // only useful for imgui window types

		// getters
		mvAppItem*                       getChild                  (const std::string& name);      // will return nullptr if not found
		inline std::vector<mvAppItem*>&  getChildren               ()       { return m_children; }
		inline mvAppItem*                getParent                 ()       { return m_parent; }   // can return nullptr
		[[nodiscard]] const std::string& getName                   () const { return m_name; }
		[[nodiscard]] const std::string& getTip                    () const { return m_tip; }
		[[nodiscard]] const std::string& getCallback               () const { return m_callback; }
		[[nodiscard]] const std::string& getPopup                  () const { return m_popup; }
		[[nodiscard]] const std::string& getDataSource             () const { return m_dataSource; }
		[[nodiscard]] int                getWidth                  () const { return m_width; }
		[[nodiscard]] int                getHeight                 () const { return m_height; }
		[[nodiscard]] bool               isShown                   () const { return m_show; }
		[[nodiscard]] bool               isContainer               () const { return m_container; }
		[[nodiscard]] bool               isItemHovered             () const { return m_hovered; }
		[[nodiscard]] bool               isItemActive              () const { return m_active; }
		[[nodiscard]] bool               isItemFocused             () const { return m_focused; }
		[[nodiscard]] bool               isItemClicked             () const { return m_clicked; }
		[[nodiscard]] bool               isItemVisible             () const { return m_visible; }
		[[nodiscard]] bool               isItemEdited              () const { return m_edited; }
		[[nodiscard]] bool               isItemActivated           () const { return m_activated; }
		[[nodiscard]] bool               isItemDeactivated         () const { return m_deactivated; }
		[[nodiscard]] bool               isItemDeactivatedAfterEdit() const { return m_deactivatedAfterEdit; }
		[[nodiscard]] bool               isItemToogledOpen         () const { return m_toggledOpen; }
		[[nodiscard]] mvVec2             getItemRectMin            () const { return m_rectMin; }
		[[nodiscard]] mvVec2             getItemRectMax            () const { return m_rectMax; }
		[[nodiscard]] mvVec2             getItemRectSize           () const { return m_rectSize; }

		// setters
		void         setParent              (mvAppItem* parent);
		void         showAll                ();
		void         hideAll                ();
		void         addChild               (mvAppItem* child);
		inline void  show                   ()                            { m_show = true; }
		inline void  hide                   ()                            { m_show = false; }
		inline void  setCallback            (const std::string& callback) { m_callback = callback; }
		inline void  setPopup               (const std::string& popup)    { m_popup = popup; }
		inline void  setTip                 (const std::string& tip)      { m_tip = tip; }
		virtual void setWidth               (int width)                   { m_width = width; }
		virtual void setHeight              (int height)                  { m_height = height; }
		inline void  setHovered             (bool value)                  { m_hovered = value; }
		inline void  setActive              (bool value)                  { m_active = value; }
		inline void  setFocused             (bool value)                  { m_focused = value; }
		inline void  setClicked             (bool value)                  { m_clicked = value; }
		inline void  setVisible             (bool value)                  { m_visible = value; }
		inline void  setEdited              (bool value)                  { m_edited = value; }
		inline void  setActivated           (bool value)                  { m_activated = value; }
		inline void  setDeactivated         (bool value)                  { m_deactivated = value; }
		inline void  setDeactivatedAfterEdit(bool value)                  { m_deactivatedAfterEdit = value; }
		inline void  setToggledOpen         (bool value)                  { m_toggledOpen = value; }
		inline void  setRectMin             (mvVec2 value)                { m_rectMin = value; }
		inline void  setRectMax             (mvVec2 value)                { m_rectMax = value; }
		inline void  setRectSize            (mvVec2 value)                { m_rectSize = value; }
		inline void  setDataSource          (const std::string& value)    { m_dataSource = value; }

	protected:

		std::string                 m_dataSource;
        std::string                 m_name;
        std::string                 m_label;
        std::string                 m_popup;
        std::string                 m_tip;
        std::string                 m_callback;
		int                         m_width  = 0;
		int                         m_height = 0;
		bool                        m_show                 = true; // determines whether to attempt rendering
		bool                        m_container            = false;
		bool                        m_hovered              = false;
		bool                        m_active               = false;
		bool                        m_focused              = false;
		bool                        m_clicked              = false;
		bool                        m_visible              = false;
		bool                        m_edited               = false;
		bool                        m_activated            = false;
		bool                        m_deactivated          = false;
		bool                        m_deactivatedAfterEdit = false;
		bool                        m_toggledOpen          = false;
		mvAppItem*                  m_parent               = nullptr;
		std::vector<mvAppItem*>     m_children;
		std::map<ImGuiCol, mvColor> m_colorStyles;
		mvVec2                      m_rectMin = {0.0f, 0.0f};
		mvVec2                      m_rectMax = {0.0f, 0.0f};
		mvVec2                      m_rectSize = {0.0f, 0.0f};

	};

}