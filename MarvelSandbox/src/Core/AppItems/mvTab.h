#pragma once

#include "Core/AppItems/mvTypeBases.h"
#include "Core/mvApp.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvTabBar
//     * mvEndTabBar
//     * mvTab
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvTabBar
	//-----------------------------------------------------------------------------
	class mvTabBar : public mvStringItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::TabBar)

		mvTabBar(const std::string& parent, const std::string& name)
			: mvStringItemBase(parent, name, "")
		{}

		virtual void draw() override
		{
			mvApp::GetApp()->pushParent(this);
			if (ImGui::BeginTabBar(m_label.c_str()))
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

				ImGui::EndTabBar();
			}
		}


	};

	//-----------------------------------------------------------------------------
	// mvEndTabBar
	//-----------------------------------------------------------------------------
	class mvEndTabBar : public mvBoolItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::EndTabBar)

		mvEndTabBar(const std::string& parent)
			: mvBoolItemBase(parent, "temporary", false)
		{
			static int i = 0;
			i++;

			m_name = "endTabBar" + std::to_string(i);
			m_show = true;

		}

		virtual void draw() override
		{
			mvApp::GetApp()->popParent();
			ImGui::EndTabBar();
		}

	};

	//-----------------------------------------------------------------------------
	// mvTab
	//-----------------------------------------------------------------------------
	class mvTab : public mvBoolItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::TabItem)

		mvTab(const std::string& parent, const std::string& name)
			: mvBoolItemBase(parent, name, false)
		{
		}

		virtual void draw() override
		{
			// cast parent to mvTabBar
			mvTabBar* parent = static_cast<mvTabBar*>(m_parent);

			// check if this is first tab
			if (parent->getValue() == "")
			{

				// set mvTabBar value to the first tab name
				parent->setValue(m_name);
				m_value = true;

			}

			// create tab item and see if it is selected
			if (ImGui::BeginTabItem(m_label.c_str()))
			{

				bool changed = false;

				// change tab bar value to this selected tab
				if (parent->getValue() != m_name)
					changed = true;
				else
					changed = false;

				parent->setValue(m_name);

				// set other tab's value false
				for (mvAppItem* child : parent->getChildren())
					static_cast<mvTab*>(child)->setValue(false);

				// set current tab value true
				m_value = true;

				showAll();

				// run call back if it exists
				if (changed)
				{
					mvApp::GetApp()->triggerCallback(m_callback, m_name);

					// Context Menu
					if (getPopup() != "")
						ImGui::OpenPopup(getPopup().c_str());
				}

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

				ImGui::EndTabItem();
			}
		}

	};

}