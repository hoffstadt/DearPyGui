#pragma once

#include "Core/AppItems/mvTypeBases.h"

namespace Marvel {

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

}