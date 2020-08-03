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
	class mvMenuBar : public mvBoolItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::MenuBar)

		mvMenuBar(const std::string& name)
			: mvBoolItemBase("", name, true)
		{
			m_container = true;
			
			// TODO use code below to set item height when font and scale systems are done
			//float FontSize = ImGui::GetFontSize(); // = Base Font Size * Current Window Scale
			//ImGuiStyle currentStyle = ImGui::GetStyle(); // = Padding for the Top and Bottom
			//m_height = int(currentStyle.FramePadding.y * 2 + FontSize);
			m_height = 21;
		}

		virtual void draw() override
		{
			if (ImGui::BeginMenuBar())
			{
				for (mvAppItem* item : m_children)
				{
					// skip item if it's not shown
					if (!item->isShown())
						continue;

					// set item width
					if (item->getWidth() > 0)
						ImGui::SetNextItemWidth((float)item->getWidth());

					item->draw();

					// Regular Tooltip (simple)
					if (item->getTip() != "" && ImGui::IsItemHovered())
						ImGui::SetTooltip("%s", item->getTip().c_str());
				}
				ImGui::EndMenuBar();
			}
		}

	};

	//-----------------------------------------------------------------------------
	// mvMenu
	//-----------------------------------------------------------------------------
	class mvMenu : public mvBoolItemBase, public mvEventHandler
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Menu)

		mvMenu(const std::string& parent, const std::string& name)
			: mvBoolItemBase(parent, name, false), mvEventHandler()
		{
			m_container = true;
		}

		virtual void draw() override
		{
			// create menu and see if its selected
			if (ImGui::BeginMenu(m_label.c_str()))
			{

				// set other menus's value false on same level
				for (mvAppItem* child : m_parent->getChildren())
					static_cast<mvMenu*>(child)->setValue(false);

				// set current menu value true
				m_value = true;

				for (mvAppItem* item : m_children)
				{
					// skip item if it's not shown
					if (!item->isShown())
						continue;

					// set item width
					if (item->getWidth() > 0)
						ImGui::SetNextItemWidth((float)item->getWidth());

					item->draw();

					// Regular Tooltip (simple)
					if (item->getTip() != "" && ImGui::IsItemHovered())
						ImGui::SetTooltip("%s", item->getTip().c_str());
				}

				registerWindowFocusing();
				ImGui::EndMenu();
			}
		}

	};

	//-----------------------------------------------------------------------------
	// mvMenuItem
	//-----------------------------------------------------------------------------
	class mvMenuItem : public mvBoolItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::MenuItem)

		mvMenuItem(const std::string& parent, const std::string& name)
			: mvBoolItemBase(parent, name, false){}

		virtual void draw() override
		{
			// create menuitem and see if its selected
			if (ImGui::MenuItem(m_label.c_str(), NULL))
			{

				// set other menusitems's value false on same level
				for (mvAppItem* child : m_parent->getChildren())
					static_cast<mvMenuItem*>(child)->setValue(false);

				m_value = true;

				mvApp::GetApp()->runCallback(m_callback, m_name);

				// Context Menu
				if (getPopup() != "")
					ImGui::OpenPopup(getPopup().c_str());

			}
		}

	};

}