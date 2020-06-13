#pragma once

#include "Core/AppItems/mvTypeBases.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvChild
//     * mvGroup
//     * mvEndGroup
//     * mvCollapsingHeader
//     * mvEndCollapsingHeader
//     * mvTreeNode
//     * mvEndTreeNode
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

			mvApp::GetApp()->pushParent(this);
			
			m_value = ImGui::BeginChild(m_label.c_str(), ImVec2(float(m_width), float(m_height)), true);

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

			mvApp::GetApp()->pushParent(this);
			ImGui::BeginGroup();

			//if (m_tip != "" && ImGui::IsItemHovered())
			//	ImGui::SetTooltip(m_tip.c_str());
		}

	};

	//-----------------------------------------------------------------------------
	// mvEndGroup
	//-----------------------------------------------------------------------------
	class mvEndGroup : public mvNoneItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::EndGroup)

		mvEndGroup(const std::string& parent)
			: mvNoneItemBase(parent, "Endgroup")
		{
		}

		virtual void draw() override
		{
			if (m_width != 0)
				ImGui::PopItemWidth();

			mvApp::GetApp()->popParent();
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

		mvCollapsingHeader(const std::string& parent, const std::string& name)
			: mvBoolItemBase(parent, name, true)
		{}

		virtual void draw() override
		{

			mvApp::GetApp()->pushParent(this);
		
			// create menu and see if its selected
			if (!ImGui::CollapsingHeader(m_label.c_str(), &m_value, 0))
			{
				hideAll();
				show();
				mvApp::GetApp()->popParent();
				return;
			}

			// set current menu value true
			//m_value = true;
			showAll();

			// Context Menu
			if (getPopup() != "")
				ImGui::OpenPopup(getPopup().c_str());

		}

	};

	//-----------------------------------------------------------------------------
	// mvEndCollapsingHeader
	//-----------------------------------------------------------------------------
	class mvEndCollapsingHeader : public mvNoneItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::EndCollapsingHeader)

		mvEndCollapsingHeader(const std::string& parent)
			: mvNoneItemBase(parent, "EndCollapsingHeader")
		{}

		virtual void draw() override
		{
			mvApp::GetApp()->popParent();
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
				mvApp::GetApp()->pushParent(this);

				// set current menu value true
				m_value = true;
				showAll();

				// Context Menu
				if (getPopup() != "")
					ImGui::OpenPopup(getPopup().c_str());
			}
			else
				m_value = false;
		}

	};

	//-----------------------------------------------------------------------------
	// mvEndTreeNode
	//-----------------------------------------------------------------------------
	class mvEndTreeNode : public mvNoneItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::EndTreeNode)

		mvEndTreeNode(const std::string& parent)
			: mvNoneItemBase(parent, "EndTreeNode")
		{}

		virtual void draw() override
		{
			ImGui::TreePop();
			mvApp::GetApp()->popParent();
		}

	};

}