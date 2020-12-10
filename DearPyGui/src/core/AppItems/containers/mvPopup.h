#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	class mvPopup : public mvBoolPtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Popup, "add_popup")

		mvPopup(const std::string& name, mvAppItem* parentAddress)
			: mvBoolPtrBase(name, false, name), m_parentAddress(parentAddress)
		{
			m_description.container = true;
			if (parentAddress)
				parentAddress->m_name;
		}

		void closePopup() { m_close = true; }

		void draw() override
		{

			auto styleManager = m_styleManager.getScopedStyleManager();
			ScopedID id(m_parentAddress);

			if (m_modal)
			{
				if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(m_button))
					ImGui::OpenPopup(m_name.c_str());

				if (ImGui::BeginPopupModal(m_name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
				{

					if (m_close)
					{
						ImGui::CloseCurrentPopup();
						m_close = false;
					}

					for (mvAppItem* item : m_children)
					{
						// skip item if it's not shown
						if (!item->m_show)
							continue;

						// set item width
						if (item->m_width != 0)
							ImGui::SetNextItemWidth((float)item->m_width);

						item->draw();

						// Regular Tooltip (simple)
						if (!item->m_tip.empty() && ImGui::IsItemHovered())
							ImGui::SetTooltip("%s", item->m_tip.c_str());

						item->getState().update();
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
						if (!item->m_show)
							continue;

						// set item width
						if (item->m_width > 0)
							ImGui::SetNextItemWidth((float)item->m_width);

						item->draw();

						item->getState().update();
					}

					// allows this item to have a render callback
					registerWindowFocusing();

					ImGui::EndPopup();
				}
			}
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
		bool m_close = false;

	};

}