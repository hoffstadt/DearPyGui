#pragma once

#include "Core/AppItems/mvTypeBases.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvPopup
//     * mvEndPopup
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvPopup
	//-----------------------------------------------------------------------------
	class mvPopup : public mvBoolItemBase, public mvEventHandler
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Popup)

		mvPopup(const std::string& parent, const std::string& name, int mousebutton = 1, bool modal=false)
			: mvBoolItemBase(parent, name, false), mvEventHandler(), m_modal(modal), m_button(mousebutton)
		{
			m_container = true;
			if (m_parent)
				m_parent->setPopup(name);
		}

		void draw() override
		{

			pushColorStyles();

			if (m_modal)
			{
				if (ImGui::BeginPopupModal(m_name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
				{

					// Context Menu
					if (!getPopup().empty())
						ImGui::OpenPopup(getPopup().c_str());

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

					ImGui::EndPopup();
				}
			}

			else 
			{
				if (ImGui::BeginPopupContextItem(m_name.c_str(), m_button))
				{

					// Context Menu
					if (!getPopup().empty())
						ImGui::OpenPopup(getPopup().c_str());

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

					ImGui::EndPopup();
				}
			}

			popColorStyles();
		}

	private:

		bool m_modal;
		int  m_button;

	};

}