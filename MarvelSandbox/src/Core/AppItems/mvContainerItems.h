#pragma once

#include "Core/AppItems/mvTypeBases.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvChild
//     * mvEndChild
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
			ImGui::BeginChild(m_label.c_str(), ImVec2(float(m_width), float(m_height)), true);

			mvApp::GetApp()->pushParent(this);

			// set current child value true
			m_value = true;

			if (m_tip != "" && ImGui::IsItemHovered())
				ImGui::SetTooltip(m_tip.c_str());
		}

	private:

		int m_height;

	};

	//-----------------------------------------------------------------------------
	// mvEndChild
	//-----------------------------------------------------------------------------
	class mvEndChild : public mvNoneItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::EndChild)

		mvEndChild(const std::string& parent)
			: mvNoneItemBase(parent, "Endgroup")
		{
		}

		virtual void draw() override
		{
			mvApp::GetApp()->popParent();
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
			: mvBoolItemBase(parent, name, false)
		{}

		virtual void draw() override
		{
			// create menu and see if its selected
			if (ImGui::CollapsingHeader(m_label.c_str(), 0))
			{
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