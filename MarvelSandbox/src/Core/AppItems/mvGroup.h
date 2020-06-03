#pragma once

#include "Core/mvAppItem.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvGroup
	//-----------------------------------------------------------------------------
	class mvGroup : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Group)
		MV_NORETURN_VALUE()

		mvGroup(const std::string& parent, const std::string& name)
			: mvAppItem(parent, name)
		{
		}

		virtual void draw() override
		{
			if (m_width != 0)
				ImGui::PushItemWidth((float)m_width);

			mvApp::GetApp()->pushParent(this);
			ImGui::BeginGroup();

			if (m_tip != "" && ImGui::IsItemHovered())
				ImGui::SetTooltip(m_tip.c_str());
		}

	};

	//-----------------------------------------------------------------------------
	// mvEndGroup
	//-----------------------------------------------------------------------------
	class mvEndGroup : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::EndGroup)
		MV_NORETURN_VALUE()

		mvEndGroup(const std::string& parent)
			: mvAppItem(parent, "Endgroup")
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