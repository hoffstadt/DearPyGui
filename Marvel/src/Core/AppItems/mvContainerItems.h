#pragma once

#include "Core/AppItems/mvTypeBases.h"
#include "Core/mvEventHandler.h"
#include "Core/mvInput.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvChild
//     * mvGroup
//     * mvCollapsingHeader
//     * mvTreeNode
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvChild
	//-----------------------------------------------------------------------------
	class mvChild : public mvBoolItemBase, public mvEventHandler
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Child)

		mvChild(const std::string& parent, const std::string& name)
			: mvBoolItemBase(parent, name, false), mvEventHandler()
		{
			m_container = true;
		}

		virtual void draw() override
		{
			
			ImGui::BeginChild(m_label.c_str(), ImVec2(float(m_width), float(m_height)), true);

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
				if (item->getTip() != "" && ImGui::IsItemHovered())
					ImGui::SetTooltip(item->getTip().c_str());


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

			// TODO check if these work for child
			if (m_tip != "" && ImGui::IsItemHovered())
				ImGui::SetTooltip(m_tip.c_str());

			if (ImGui::IsWindowFocused())
			{

				// update mouse
				ImVec2 mousePos = ImGui::GetMousePos();
				float x = mousePos.x - ImGui::GetWindowPos().x;
				float y = mousePos.y - ImGui::GetWindowPos().y;
				mvInput::setMousePosition(x, y);
				mvApp::GetApp()->setActiveWindow(m_name);

			}

			ImGui::EndChild();
		}

	};

	//-----------------------------------------------------------------------------
	// mvGroup
	//-----------------------------------------------------------------------------
	class mvGroup : public mvNoneItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Group)

		mvGroup(const std::string& parent, const std::string& name)
			: mvNoneItemBase(parent, name)
		{
			m_container = true;
		}

		virtual void draw() override
		{
			if (m_width != 0)
				ImGui::PushItemWidth((float)m_width);

			ImGui::BeginGroup();

			for (mvAppItem* item : m_children)
			{
				if (m_width != 0)
					item->setWidth(m_width);

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
				if (item->getTip() != "" && ImGui::IsItemHovered())
					ImGui::SetTooltip(item->getTip().c_str());


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

			if (m_width != 0)
				ImGui::PopItemWidth();

			ImGui::EndGroup();

		}

	};

	//-----------------------------------------------------------------------------
	// mvCollapsingHeader
	//-----------------------------------------------------------------------------
	class mvCollapsingHeader : public mvBoolItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::CollapsingHeader)

		mvCollapsingHeader(const std::string& parent, const std::string& name, ImGuiTreeNodeFlags flags = 0)
			: mvBoolItemBase(parent, name, true), m_flags(flags)
		{
			m_container = true;
		}

		virtual void draw() override
		{
		
			// create menu and see if its selected
			if (ImGui::CollapsingHeader(m_label.c_str(), &m_value, m_flags))
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
					if (item->getTip() != "" && ImGui::IsItemHovered())
						ImGui::SetTooltip(item->getTip().c_str());


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
			}

		}

	private:

		ImGuiTreeNodeFlags m_flags;

	};

	//-----------------------------------------------------------------------------
	// mvTreeNode
	//-----------------------------------------------------------------------------
	class mvTreeNode : public mvBoolItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::TreeNode)

		mvTreeNode(const std::string& parent, const std::string& name, ImGuiTreeNodeFlags flags = 0)
			: mvBoolItemBase(parent, name, false), m_flags(flags)
		{
			m_container = true;
		}

		virtual void draw() override
		{
			ImGui::BeginGroup();
			if (ImGui::TreeNodeEx(m_label.c_str(), m_flags))
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
					if (item->getTip() != "" && ImGui::IsItemHovered())
						ImGui::SetTooltip(item->getTip().c_str());


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

				ImGui::TreePop();
			}
			ImGui::EndGroup();
		}

	private:

		ImGuiTreeNodeFlags m_flags;

	};

}