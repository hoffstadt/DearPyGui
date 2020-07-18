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
			: mvBoolItemBase(parent, name, false)
		{
			// TODO fix parent ugliness

			// has to be showed that way it can check for hovering
			// otherwise it will never show
			m_show = true;
			m_container = true;
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

					item->pushColorStyles();
					item->draw();
					item->popColorStyles();


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
				ImGui::EndTooltip();
			}

		}

	};

}