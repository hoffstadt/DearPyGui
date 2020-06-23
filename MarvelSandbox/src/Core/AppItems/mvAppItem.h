#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <string>
#include <vector>
#include <map>
#include <imgui.h>
#include <mutex>
#include "Core/mvCore.h"

//-----------------------------------------------------------------------------
// Helper Macro
//-----------------------------------------------------------------------------
#define MV_APPITEM_TYPE(x) virtual mvAppItemType getType() const override { return x; }

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
		Indent, Unindent, Drawing, Window, EndWindow,
		Popup, EndPopup, Selectable, TreeNode, ProgressBar
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
		mvAppItem operator=(const mvAppItem& other) = delete; // copy assignment operator
		mvAppItem operator=(mvAppItem&& other)      = delete; // move assignment operator

		// pure virtual methods
		virtual void          setPyValue(PyObject* value) = 0;
		virtual PyObject*     getPyValue()          const = 0;
		virtual mvAppItemType getType   ()          const = 0;
		virtual void          draw      ()                = 0;

		// color styles for runtime (WORK NEEDED HERE)
		void addColorStyle  (ImGuiCol item, mvColor color);
		void pushColorStyles();
		void popColorStyles ();

		// getters
		mvAppItem*                      getChild                  (const std::string& name);      // will return nullptr if not found
		inline std::vector<mvAppItem*>& getChildren               ()       { return m_children; }
		inline mvAppItem*               getParent                 ()       { return m_parent; }   // can return nullptr
		inline const std::string&       getName                   () const { return m_name; }
		inline const std::string&       getTip                    () const { return m_tip; }
		inline const std::string&       getCallback               () const { return m_callback; }
		inline const std::string&       getPopup                  () const { return m_popup; }
		inline int                      getWidth                  () const { return m_width; }
		inline int                      getHeight                 () const { return m_height; }
		inline bool                     isShown                   () const { return m_show; }
		inline bool                     isContainer               () const { return m_container; }
		inline bool                     isItemHovered             () const { return m_hovered; }
		inline bool                     isItemActive              () const { return m_active; }
		inline bool                     isItemFocused             () const { return m_focused; }
		inline bool                     isItemClicked             () const { return m_clicked; }
		inline bool                     isItemVisible             () const { return m_visible; }
		inline bool                     isItemEdited              () const { return m_edited; }
		inline bool                     isItemActivated           () const { return m_activated; }
		inline bool                     isItemDeactivated         () const { return m_deactivated; }
		inline bool                     isItemDeactivatedAfterEdit() const { return m_deactivatedAfterEdit; }
		inline bool                     isItemToogledOpen         () const { return m_toggledOpen; }
		inline mvVec2                   getItemRectMin            () const { return m_rectMin; }
		inline mvVec2                   getItemRectMax            () const { return m_rectMax; }
		inline mvVec2                   getItemRectSize           () const { return m_rectSize; }

		// setters
		void        setParent              (mvAppItem* parent);
		void        showAll                ();
		void        hideAll                ();
		void        addChild               (mvAppItem* child);
		inline void show                   ()                            { m_show = true; }
		inline void hide                   ()                            { m_show = false; }
		inline void setCallback            (const std::string& callback) { m_callback = callback; }
		inline void setPopup               (const std::string& popup)    { m_popup = popup; }
		inline void setTip                 (const std::string& tip)      { m_tip = tip; }
		inline void setWidth               (int width)                   { m_width = width; }
		inline void setHeight              (int height)                  { m_height = height; }
		inline void setHovered             (bool value)                  { m_hovered = value; }
		inline void setActive              (bool value)                  { m_active = value; }
		inline void setFocused             (bool value)                  { m_focused = value; }
		inline void setClicked             (bool value)                  { m_clicked = value; }
		inline void setVisible             (bool value)                  { m_visible = value; }
		inline void setEdited              (bool value)                  { m_edited = value; }
		inline void setActivated           (bool value)                  { m_activated = value; }
		inline void setDeactivated         (bool value)                  { m_deactivated = value; }
		inline void setDeactivatedAfterEdit(bool value)                  { m_deactivatedAfterEdit = value; }
		inline void setToggledOpen         (bool value)                  { m_toggledOpen = value; }
		inline void setRectMin             (mvVec2 value)                { m_rectMin = value; }
		inline void setRectMax             (mvVec2 value)                { m_rectMax = value; }
		inline void setRectSize            (mvVec2 value)                { m_rectSize = value; }

	protected:

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
		std::string                 m_name;
		std::string                 m_label    = "";
		std::string                 m_popup    = "";
		std::string                 m_tip      = "";
		std::string                 m_callback = "";
		mvAppItem*                  m_parent = nullptr;
		std::vector<mvAppItem*>     m_children;
		std::map<ImGuiCol, mvColor> m_colorStyles;
		mvVec2                      m_rectMin;
		mvVec2                      m_rectMax;
		mvVec2                      m_rectSize;
		mutable std::mutex          m_rmutex; // read mutex
		mutable std::mutex          m_wmutex; // write mutex

	};

}