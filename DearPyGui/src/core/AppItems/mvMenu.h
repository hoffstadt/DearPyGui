#pragma once

#include "core/AppItems/mvTypeBases.h"
#include "mvApp.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvMenuBar
//     * mvMenu
//     * mvMenuItem
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvMenuBar
	//-----------------------------------------------------------------------------
	class mvMenuBar : public mvPtrBase<bool, 1>
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::MenuBar, "add_menu_bar")

		explicit mvMenuBar(const std::string& name)
			: mvPtrBase<bool, 1>(name, true, name)
		{
			m_description.container = true;
			
			// TODO use code below to set item height when font and scale systems are done
			//float FontSize = ImGui::GetFontSize(); // = Base Font Size * Current Window Scale
			//ImGuiStyle currentStyle = ImGui::GetStyle(); // = Padding for the Top and Bottom
			//m_height = int(currentStyle.FramePadding.y * 2 + FontSize);
			m_height = 21;
		}

		void draw() override
		{
			auto styleManager = m_styleManager.getScopedStyleManager();

			if (ImGui::BeginMenuBar())
			{
				for (mvAppItem* item : m_children)
				{
					// skip item if it's not shown
					if (!item->isShown())
						continue;

					// set item width
					if (item->getWidth() != 0)
						ImGui::SetNextItemWidth((float)item->getWidth());

					item->draw();

					// Regular Tooltip (simple)
					if (!item->getTip().empty() && ImGui::IsItemHovered())
						ImGui::SetTooltip("%s", item->getTip().c_str());

					item->getState().update();
				}
				ImGui::EndMenuBar();
			}

		}

	};

	//-----------------------------------------------------------------------------
	// mvMenu
	//-----------------------------------------------------------------------------
	class mvMenu : public mvPtrBase<bool, 1>, public mvEventHandler
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Menu, "add_menu")

		mvMenu(const std::string& name)
			: mvPtrBase<bool, 1>(name, false, name), mvEventHandler()
		{
			m_description.container = true;
		}

		void draw() override
		{
			auto styleManager = m_styleManager.getScopedStyleManager();
			ScopedID id;

			// create menu and see if its selected
			if (ImGui::BeginMenu(m_label.c_str(), m_enabled))
			{

				// set other menus's value false on same level
                for (mvAppItem* sibling : m_parent->getChildren())
                {
                    // ensure sibling
                    if(sibling->getType() == mvAppItemType::Menu)
                        *((mvMenu *) sibling)->m_value = false;
                }

				// set current menu value true
				*m_value = true;

				for (mvAppItem* item : m_children)
				{
					// skip item if it's not shown
					if (!item->isShown())
						continue;

					// set item width
					if (item->getWidth() != 0)
						ImGui::SetNextItemWidth((float)item->getWidth());

					item->draw();

					// Regular Tooltip (simple)
					if (!item->getTip().empty() && ImGui::IsItemHovered())
						ImGui::SetTooltip("%s", item->getTip().c_str());

					item->getState().update();
				}

				registerWindowFocusing();

				ImGui::EndMenu();
			}

		}

		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "enabled")) m_enabled = ToBool(item);

		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "enabled", ToPyBool(m_enabled));
		}

	private:

		bool m_enabled = true;

	};

	//-----------------------------------------------------------------------------
	// mvMenuItem
	//-----------------------------------------------------------------------------
	class mvMenuItem : public mvPtrBase<bool, 1>
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::MenuItem, "add_menu_item")

		mvMenuItem(const std::string& name)
			: mvPtrBase<bool, 1>(name, false, name)
		{
		}

		void draw() override
		{
			auto styleManager = m_styleManager.getScopedStyleManager();
			ScopedID id;

			// create menuitem and see if its selected
			if (ImGui::MenuItem(m_label.c_str(), m_shortcut.c_str(), m_check ? m_value : nullptr, m_enabled))
			{

				// set other menuitems's value false on same level
				for (mvAppItem* sibling : m_parent->getChildren())
				{
				    // ensure sibling
				    if(sibling->getType() == mvAppItemType::MenuItem)
                        *((mvMenuItem *) sibling)->m_value = false;
                }

				*m_value = true;

				mvApp::GetApp()->addCallback(m_callback, m_name, m_callbackData);

			}

		}

		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "shortcut")) m_shortcut = ToString(item);
			if (PyObject* item = PyDict_GetItemString(dict, "check")) m_check = ToBool(item);

		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "shortcut", ToPyString(m_shortcut));
			PyDict_SetItemString(dict, "check", ToPyBool(m_check));
		}

	private:

		std::string m_shortcut;
		bool        m_check = false;

	};

}