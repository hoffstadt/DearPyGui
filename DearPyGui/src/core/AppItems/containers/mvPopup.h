#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	class mvPopup : public mvBoolPtrBase
	{
		MV_APPITEM_TYPE(mvAppItemType::Popup, mvPopup, "add_popup")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::Popup, mvThemeCol_Popup_TitleText				,  0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Popup, mvThemeCol_Popup_TitleBg					, 11L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Popup, mvThemeCol_Popup_Bg						,  4L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Popup, mvThemeCol_Popup_ModalDimBg				, 49L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Popup, mvThemeCol_Popup_Scrollbar				, 14L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Popup, mvThemeCol_Popup_ScrollbarGrab			, 15L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Popup, mvThemeCol_Popup_ScrollbarGrabHovered	, 16L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Popup, mvThemeCol_Popup_ScrollbarGrabActive		, 17L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Popup, mvThemeCol_Popup_Border					,  5L);


		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Popup_TitleText),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Popup_TitleBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Popup_Bg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Popup_ModalDimBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Popup_Scrollbar),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Popup_ScrollbarGrab),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Popup_ScrollbarGrabHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Popup_ScrollbarGrabActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Popup_Border),
		MV_END_COLOR_CONSTANTS

	public:

		mvPopup(const std::string& name, mvAppItem* parentAddress)
			: mvBoolPtrBase(name, false, name), m_parentAddress(parentAddress)
		{
			m_description.container = true;
		}

		void closePopup() { m_close = true; }

		void draw() override
		{

			auto styleManager = m_styleManager.getScopedStyleManager();
			ScopedID id(m_parentAddress);
			mvImGuiThemeScope scope(this);

			if (m_modal)
			{
				if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(m_button))
					ImGui::OpenPopup(m_core_config.name.c_str());

				if (ImGui::BeginPopupModal(m_core_config.name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
				{

					if (m_close)
					{
						ImGui::CloseCurrentPopup();
						m_close = false;
					}

					//we do this so that the children dont get the theme
					scope.cleanup();

					for (mvRef<mvAppItem> item : m_children)
					{
						// skip item if it's not shown
						if (!item->m_core_config.show)
							continue;

						// set item width
						if (item->m_core_config.width != 0)
							ImGui::SetNextItemWidth((float)item->m_core_config.width);

						item->draw();

						item->getState().update();
					}

					ImGui::EndPopup();
				}
			}

			else 
			{
				if (ImGui::BeginPopupContextItem(m_core_config.name.c_str(), m_button))
				{

					//we do this so that the children dont get the theme
					scope.cleanup();

					for (mvRef<mvAppItem> item : m_children)
					{
						// skip item if it's not shown
						if (!item->m_core_config.show)
							continue;

						// set item width
						if (item->m_core_config.width > 0)
							ImGui::SetNextItemWidth((float)item->m_core_config.width);

						item->draw();

						item->getState().update();
					}

					// allows this item to have a render callback
					registerWindowFocusing();

					ImGui::EndPopup();
				}
			}
		}

#ifndef MV_CPP

		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			 
			if (PyObject* item = PyDict_GetItemString(dict, "modal")) m_modal = ToBool(item);
			if (PyObject* item = PyDict_GetItemString(dict, "mousebutton")) m_button = ToInt(item);

		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			 
			PyDict_SetItemString(dict, "modal", ToPyBool(m_modal));
			PyDict_SetItemString(dict, "mousebutton", ToPyInt(m_button));
		}

#endif

	private:

		bool m_modal = false;
		int  m_button = 1;
		mvAppItem* m_parentAddress = nullptr;
		bool m_close = false;

	};

}