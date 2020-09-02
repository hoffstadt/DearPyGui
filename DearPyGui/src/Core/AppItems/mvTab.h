#pragma once

#include "Core/AppItems/mvTypeBases.h"
#include "mvApp.h"

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

		mvTabBar(const std::string& parent, const std::string& name, bool reorderable = false)
			: mvStringItemBase(parent, name, "")
		{
			m_container = true;
			if (reorderable)
				m_flags |= ImGuiTabBarFlags_Reorderable;
		
		}

		void draw() override
		{
			if (ImGui::BeginTabBar(m_label.c_str(), m_flags))
			{
				for (mvAppItem* item : m_children)
				{
					// skip item if it's not shown
					if (!item->isShown())
						continue;

					// set item width
					if (item->getWidth() > 0)
						ImGui::SetNextItemWidth((float)item->getWidth());

					item->pushColorStyles();
					item->draw();
					item->popColorStyles();

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

				ImGui::EndTabBar();
			}
		}

	private:

		ImGuiTabBarFlags m_flags = ImGuiTabBarFlags_None;

	};

	//-----------------------------------------------------------------------------
	// mvTab
	//-----------------------------------------------------------------------------
	class mvTab : public mvBoolItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::TabItem)

		mvTab(const std::string& parent, const std::string& name, bool closable = false)
			: mvBoolItemBase(parent, name, false), m_closable(closable)
		{
			m_container = true;
		}

		void draw() override
		{
			// cast parent to mvTabBar
			auto parent = (mvTabBar*)m_parent;

			// check if this is first tab
			if (parent->getValue().empty())
			{
				// set mvTabBar value to the first tab name
				parent->setValue(m_name);
				m_value = true;
			}

			// create tab item and see if it is selected
			if (ImGui::BeginTabItem(m_label.c_str(), m_closable ? &m_show : nullptr))
			{
				parent->setValue(m_name);

				// set other tab's value false
				for (mvAppItem* child : parent->getChildren())
                    ((mvTab*)child)->setValue(false);

				// set current tab value true
				m_value = true;

				showAll();

				// run call back if it exists
				if (parent->getValue() != m_name)
				{
					mvApp::GetApp()->runCallback(parent->getCallback(), m_name);

					// Context Menu
					if (!getPopup().empty())
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

					item->pushColorStyles();
					item->draw();
					item->popColorStyles();

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

				ImGui::EndTabItem();
			}
		}

	private:

		bool m_closable = false;

	};

}