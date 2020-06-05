#pragma once

#include "Core/AppItems/mvtypeBases.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvTooltip
	//-----------------------------------------------------------------------------
	class mvTooltip : public mvBoolItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Tooltip)

		mvTooltip(const std::string& parent, const std::string& name)
			: mvBoolItemBase(parent, name, false)
		{
			// has to be showed that way it can check for hovering
			// otherwise it will never show
			m_show = true;
		}

		virtual void draw() override
		{
			if (ImGui::IsItemHovered() &&
				mvApp::GetApp()->topParent() == getParent()->getParent()
				|| mvApp::GetApp()->topParent() == getParent())
			{
				mvApp::GetApp()->pushParent(this);

				ImGui::BeginTooltip();

				// set current menu value true
				m_value = true;
			}
			else
				m_value = false;
		}

	};

	//-----------------------------------------------------------------------------
	// mvEndTooltip
	//-----------------------------------------------------------------------------
	class mvEndTooltip : public mvNoneItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::EndTooltip)

		mvEndTooltip(const std::string& parent)
			: mvNoneItemBase(parent, "EndTooltip")
		{
		}

		virtual void draw() override
		{
			mvApp::GetApp()->popParent();
			ImGui::EndTooltip();
		}

	};

}