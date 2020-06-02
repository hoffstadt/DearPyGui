#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <string>
#include <vector>
#include <functional>

namespace Marvel {

	enum class mvAppItemType
	{
		None = 0, SampleWidget,
		InputText, InputFloat, InputFloat3, InputInt, Button, Checkbox, RadioButtons, Combo,
		Listbox, CollapsingHeader, TabBar, TabItem, EndTabItem, EndTabBar, Group, EndGroup,
		MenuBar, Menu, EndMenu, MenuItem, EndMenuBar, Spacing, Separator, SameLine,
		Tooltip, EndTooltip, Text, LabelText, ProgressBar, Child, EndChild, Custom, Image,
		DragInt, ColorEdit4, SliderInt, FileOpen, FileSave
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

		virtual mvAppItemType getType() const = 0;
		virtual void          draw() = 0;

		inline const std::string&       getName() const { return m_name; }
		inline std::vector<mvAppItem*>& getChildren() { return m_children; }
		inline mvAppItem*               getParent() { return m_parent; }
		inline bool                     isShown() const { return m_show; }
		inline void                     show() { m_show = true; }
		inline void                     hide() { m_show = false; }
		inline void                     setCallback(const std::string& callback) { m_callback = callback; }
		inline const std::string&       getCallback() { return m_callback; }

		void showAll();
		void hideAll();

	protected:

		std::string             m_name;
		std::string             m_label;
		std::string             m_tip;
		bool                    m_show;
		mvAppItem*              m_parent = nullptr;
		std::vector<mvAppItem*> m_children;
		std::string             m_callback = "";

	};

}