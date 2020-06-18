#pragma once

#include "Core/AppItems/mvtypeBases.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvTooltip
//     * mvEndTooltip
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvTooltip
	//-----------------------------------------------------------------------------
	class mvTooltip : public mvBoolItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Tooltip)

		mvTooltip(const std::string& parent, const std::string& name)
			: mvBoolItemBase(mvApp::GetApp()->getItem(parent)->getParent()->getName(), name, false)
		{
			// TODO fix parent ugliness

			// has to be showed that way it can check for hovering
			// otherwise it will never show
			m_show = true;
		}

		virtual void draw() override
		{
			if (ImGui::IsItemHovered())
			{

				ImGui::BeginTooltip();
				for (mvAppItem* item : m_children)
				{
					// skip item if it's not shown
					if (!item->isShown())
						continue;

					// set item width
					if (item->getWidth() > 0)
						ImGui::SetNextItemWidth((float)item->getWidth());

					item->draw();

				}
				ImGui::EndTooltip();

			}

		}

	};

}