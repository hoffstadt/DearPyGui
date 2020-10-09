#pragma once

#include "Core/AppItems/mvTypeBases.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvPopup
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvPopup
	//-----------------------------------------------------------------------------
	class mvPopup : public mvBoolPtrBase, public mvEventHandler
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Popup, "add_popup")

		mvPopup(const std::string& name, mvAppItem* parentAddress)
			: mvBoolPtrBase(name, false, name), mvEventHandler(), m_parentAddress(parentAddress)
		{
			m_container = true;
			//if (m_parent)
				//m_parent->setPopup(name);
		}

		void draw() override
		{

			pushColorStyles();
			ImGui::PushID(m_parentAddress);

			if (m_modal)
			{
				if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(m_button))
					ImGui::OpenPopup(m_name.c_str());

				if (ImGui::BeginPopupModal(m_name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
				{

					for (mvAppItem* item : m_children)
					{
						// skip item if it's not shown
						if (!item->isShown())
							continue;

						// set item width
						if (item->getWidth() != 0)
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

					// allows this item to have a render callback
					registerWindowFocusing();

					ImGui::EndPopup();
				}
			}

			ImGui::PopID();
			popColorStyles();
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "modal")) m_modal = ToBool(item);
			if (PyObject* item = PyDict_GetItemString(dict, "mousebutton")) m_button = ToInt(item);

		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "modal", ToPyBool(m_modal));
			PyDict_SetItemString(dict, "mousebutton", ToPyInt(m_button));
		}

	private:

		bool m_modal = false;
		int  m_button = 1;
		mvAppItem* m_parentAddress = nullptr;

	};

}