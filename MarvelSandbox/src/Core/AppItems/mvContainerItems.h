#pragma once

#include "Core/AppItems/mvTypeBases.h"

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
	class mvChild : public mvBoolItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Child)

		mvChild(const std::string& parent, const std::string& name, int width, int height)
			: mvBoolItemBase(parent, name, false)
		{
			m_width = width;
			m_height = height;
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
			}

			// TODO check if these work for child
			if (m_tip != "" && ImGui::IsItemHovered())
				ImGui::SetTooltip(m_tip.c_str());

			ImGui::EndChild();
		}

	private:

		int m_height;

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
		{}

		virtual void draw() override
		{
			if (m_width != 0)
				ImGui::PushItemWidth((float)m_width);

			ImGui::BeginGroup();

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
			}

			if (m_width != 0)
				ImGui::PopItemWidth();

			ImGui::EndGroup();

			//if (m_tip != "" && ImGui::IsItemHovered())
			//	ImGui::SetTooltip(m_tip.c_str());
		}

	};

	//-----------------------------------------------------------------------------
	// mvCollapsingHeader
	//-----------------------------------------------------------------------------
	class mvCollapsingHeader : public mvBoolItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::CollapsingHeader)

		mvCollapsingHeader(const std::string& parent, const std::string& name)
			: mvBoolItemBase(parent, name, true)
		{}

		virtual void draw() override
		{
		
			// create menu and see if its selected
			if (ImGui::CollapsingHeader(m_label.c_str(), &m_value, 0))
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
				}
			}

			// set current menu value true
			//m_value = true;
			//showAll();

			// Context Menu
			//if (getPopup() != "")
			//	ImGui::OpenPopup(getPopup().c_str());

		}

	};

	//-----------------------------------------------------------------------------
	// mvTreeNode
	//-----------------------------------------------------------------------------
	class mvTreeNode : public mvBoolItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::TreeNode)

		mvTreeNode(const std::string& parent, const std::string& name)
			: mvBoolItemBase(parent, name, false)
		{}

		virtual void draw() override
		{
			if (ImGui::TreeNode(m_label.c_str()))
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
				}

				ImGui::TreePop();
			}
		}

	};

}