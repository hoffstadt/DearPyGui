#pragma once

#include "core/AppItems/mvTypeBases.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvTabBar
//     * mvTab
//     * mvTabButton
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvTabBar
	//-----------------------------------------------------------------------------
	class mvTabBar : public mvStringPtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::TabBar, "add_tab_bar")

		mvTabBar(const std::string& name);

		std::string& getValue          ();
		void         setValue          (const std::string& value);
		void         draw              ()               override;
		void         setExtraConfigDict(PyObject* dict) override;
		void         getExtraConfigDict(PyObject* dict) override;

	private:

		ImGuiTabBarFlags m_flags = ImGuiTabBarFlags_None;

	};

	//-----------------------------------------------------------------------------
	// mvTab
	//-----------------------------------------------------------------------------
	class mvTab : public mvBoolPtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::TabItem, "add_tab")

		mvTab(const std::string& name);

		void draw              ()               override;
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		bool              m_closable = false;
		ImGuiTabItemFlags m_flags = ImGuiTabItemFlags_None;

	};

	//-----------------------------------------------------------------------------
	// mvTabButton
	//-----------------------------------------------------------------------------
	class mvTabButton : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::TabButton, "add_tab_button")

		mvTabButton(const std::string& name);

		void draw              ()               override;
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		ImGuiTabItemFlags m_flags = ImGuiTabItemFlags_None;
	};
}