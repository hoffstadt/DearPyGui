#pragma once

#include "Core/AppItems/mvTypeBases.h"
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
	class mvMenuBar : public mvBoolPtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::MenuBar, "add_menu_bar")

		explicit mvMenuBar(const std::string& name)
			: mvBoolPtrBase(name, true, name)
		{
			m_container = true;
			
			// TODO use code below to set item height when font and scale systems are done
			//float FontSize = ImGui::GetFontSize(); // = Base Font Size * Current Window Scale
			//ImGuiStyle currentStyle = ImGui::GetStyle(); // = Padding for the Top and Bottom
			//m_height = int(currentStyle.FramePadding.y * 2 + FontSize);
			m_height = 21;
		}

		void draw() override
		{
			pushColorStyles();

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

					item->setHovered(ImGui::IsItemHovered());
					item->setActive(ImGui::IsItemActive());
					item->setFocused(ImGui::IsItemFocused());
					item->setClicked(ImGui::IsItemClicked());
					item->setVisible(ImGui::IsItemVisible());
					item->setEdited(ImGui::IsItemEdited());
					item->setActivated(ImGui::IsItemActivated());
					item->setDeactivated(ImGui::IsItemDeactivated());
					item->setDeactivatedAfterEdit(ImGui::IsItemDeactivatedAfterEdit());
					item->setToggledOpen(ImGui::IsItemToggledOpen());
					item->setRectMin({ ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y });
					item->setRectMax({ ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y });
					item->setRectSize({ ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y });
				}
				ImGui::EndMenuBar();
			}

			popColorStyles();
		}

	};

	//-----------------------------------------------------------------------------
	// mvMenu
	//-----------------------------------------------------------------------------
	class mvMenu : public mvBoolPtrBase, public mvEventHandler
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Menu, "add_menu")

		mvMenu( const std::string& name)
			: mvBoolPtrBase(name, false, name), mvEventHandler()
		{
			m_container = true;
		}

		void draw() override
		{
			pushColorStyles();

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

					item->setHovered(ImGui::IsItemHovered());
					item->setActive(ImGui::IsItemActive());
					item->setFocused(ImGui::IsItemFocused());
					item->setClicked(ImGui::IsItemClicked());
					item->setVisible(ImGui::IsItemVisible());
					item->setEdited(ImGui::IsItemEdited());
					item->setActivated(ImGui::IsItemActivated());
					item->setDeactivated(ImGui::IsItemDeactivated());
					item->setDeactivatedAfterEdit(ImGui::IsItemDeactivatedAfterEdit());
					item->setToggledOpen(ImGui::IsItemToggledOpen());
					item->setRectMin({ ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y });
					item->setRectMax({ ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y });
					item->setRectSize({ ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y });
				}

				registerWindowFocusing();

				ImGui::EndMenu();
			}

			popColorStyles();
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
	class mvMenuItem : public mvBoolPtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::MenuItem, "add_menu_item")

		mvMenuItem(const std::string& name)
			: mvBoolPtrBase(name, false, name){}

		void draw() override
		{
			pushColorStyles();

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

				mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());

			}

			popColorStyles();
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "shortcut")) m_shortcut = ToString(item);
			if (PyObject* item = PyDict_GetItemString(dict, "check")) m_check = ToBool(item);
			if (PyObject* item = PyDict_GetItemString(dict, "enabled")) m_enabled = ToBool(item);

		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "shortcut", ToPyString(m_shortcut));
			PyDict_SetItemString(dict, "check", ToPyBool(m_check));
			PyDict_SetItemString(dict, "enabled", ToPyBool(m_enabled));
		}

	private:

		std::string m_shortcut;
		bool        m_check = false;
		bool        m_enabled = true;

	};

}