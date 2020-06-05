#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <string>
#include <vector>
#include <imgui.h>
#include "mvColor1.h"

#define MV_APPITEM_TYPE(x) virtual mvAppItemType getType() const override { return x; }

namespace Marvel {

	//  InputFloat3, ProgressBar, Image, DragInt, SliderInt, FileOpen, FileSave

	enum class mvAppItemType
	{
		None = 0, Spacing, SameLine,
		InputText, Button, RadioButtons,
		TabBar, TabItem, EndTabItem, EndTabBar,
		MenuBar, Menu, EndMenu, MenuItem, EndMenuBar,
		Group, EndGroup, Child, EndChild,
		Tooltip, EndTooltip, CollapsingHeader,
		Separator, ColorEdit4, Checkbox, InputInt, InputFloat,
		Listbox, Text, LabelText, Combo, Plot
	};

	class mvAppItem
	{

	public:

		mvAppItem(const std::string& parent, const std::string& name);

		~mvAppItem() = default;

		mvAppItem(const mvAppItem& other) = delete;
		mvAppItem(mvAppItem&& other) = delete;
		mvAppItem operator=(const mvAppItem& other) = delete;
		mvAppItem operator=(mvAppItem&& other) = delete;

		virtual void          setPyValue(PyObject* value) = 0;
		virtual PyObject*     getPyValue() const = 0;
		virtual mvAppItemType getType() const = 0;
		virtual void          draw() = 0;

		inline const std::string&       getName() const { return m_name; }
		inline const std::string&       getTip() const { return m_tip; }
		inline std::vector<mvAppItem*>& getChildren() { return m_children; }
		inline mvAppItem*               getParent() { return m_parent; }
		inline bool                     isShown() const { return m_show; }
		inline void                     show() { m_show = true; }
		inline void                     hide() { m_show = false; }
		inline void                     setCallback(const std::string& callback) { m_callback = callback; }
		inline void                     setTip(const std::string& tip) { m_tip = tip; }
		inline const std::string&       getCallback() { return m_callback; }
		inline int                      getWidth() const { return m_width; }
		inline void                     setWidth(int width) { m_width = width; }

		void showAll();
		void hideAll();

	protected:

		int                     m_width = 0;
		std::string             m_name;
		std::string             m_label;
		std::string             m_tip;
		bool                    m_show;
		mvAppItem*              m_parent = nullptr;
		std::vector<mvAppItem*> m_children;
		std::string             m_callback = "";

	};

}