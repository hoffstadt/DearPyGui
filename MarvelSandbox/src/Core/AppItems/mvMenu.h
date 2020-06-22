#pragma once

#include "Core/AppItems/mvTypeBases.h"
#include "Core/mvApp.h"

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
		{}

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
						ImGui::SetTooltip(item->getTip().c_str());
				}
				ImGui::EndMenuBar();
			}
		}

	};

	//-----------------------------------------------------------------------------
	// mvMenu
	//-----------------------------------------------------------------------------
	class mvMenu : public mvBoolItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Menu)

		mvMenu(const std::string& parent, const std::string& name)
			: mvBoolItemBase(parent, name, false)
		{
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
						ImGui::SetTooltip(item->getTip().c_str());
				}

				// Context Menu
				if (getPopup() != "")
					ImGui::OpenPopup(getPopup().c_str());

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

				auto threadpool = mvThreadPool::GetThreadPool();
				threadpool->submit(std::bind(&mvApp::triggerCallback, mvApp::GetApp(), m_callback, m_name));

				// Context Menu
				if (getPopup() != "")
					ImGui::OpenPopup(getPopup().c_str());

			}
		}

	};

}