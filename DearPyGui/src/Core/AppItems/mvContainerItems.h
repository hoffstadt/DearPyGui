#pragma once

#include "Core/AppItems/mvTypeBases.h"
#include "mvEventHandler.h"
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

		mvChild(const std::string& parent, const std::string& name, bool border)
			: mvBoolItemBase(parent, name, false), mvEventHandler(), m_border(border)
		{
			m_container = true;
		}

		void draw() override
		{
			
			ImGui::BeginChild(m_label.c_str(), ImVec2(float(m_width), float(m_height)), m_border);

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

			// TODO check if these work for child
			if (!m_tip .empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", m_tip.c_str());

			// allows this item to have a render callback
			registerWindowFocusing();

			m_width = (int)ImGui::GetWindowWidth();
			m_height = (int)ImGui::GetWindowHeight();

			ImGui::EndChild();
		}

	private:

		bool m_border;

	};

	//-----------------------------------------------------------------------------
	// mvGroup
	//-----------------------------------------------------------------------------
	class mvGroup : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Group)

		mvGroup(const std::string& parent, const std::string& name, bool horizontal=false, float spacing=-1.0f)
			: mvAppItem(parent, name), m_horizontal(horizontal), m_hspacing(spacing)
		{
			m_container = true;
		}

		void draw() override
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

				if (m_horizontal)
					ImGui::SameLine(0.0, m_hspacing);

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

			if (m_width != 0)
				ImGui::PopItemWidth();

			ImGui::EndGroup();

		}

	private:

		bool  m_horizontal = false;
		float m_hspacing = -1.0f;

	};

	//-----------------------------------------------------------------------------
	// mvCollapsingHeader
	//-----------------------------------------------------------------------------
	class mvCollapsingHeader : public mvBoolItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::CollapsingHeader)

		mvCollapsingHeader(const std::string& parent, const std::string& name, ImGuiTreeNodeFlags flags = 0, bool closable = false)
			: mvBoolItemBase(parent, name, true), m_flags(flags), m_closable(closable)
		{
			m_container = true;
		}

		void draw() override
		{
			bool* toggle = nullptr;
			if (m_closable)
				toggle = &m_value;

			if (ImGui::CollapsingHeader(m_label.c_str(), toggle, m_flags))
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
			}

		}

	private:

		ImGuiTreeNodeFlags m_flags;
		bool               m_closable;

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

		void draw() override
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

				ImGui::TreePop();
			}
			ImGui::EndGroup();
		}

	private:

		ImGuiTreeNodeFlags m_flags;

	};

}