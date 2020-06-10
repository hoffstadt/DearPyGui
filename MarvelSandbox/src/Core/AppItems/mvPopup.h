#pragma once

#include "Core/AppItems/mvtypeBases.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvPopup
	//-----------------------------------------------------------------------------
	class mvPopup : public mvBoolItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Popup)

		mvPopup(const std::string& parent, const std::string& name, int mousebutton = 1, bool modal=false)
			: mvBoolItemBase(parent, name, false), m_modal(modal), m_button(mousebutton)
		{
			if (m_parent)
				m_parent->setPopup(name);
		}

		virtual void draw() override
		{
			if (m_parent == nullptr)
			{
				if (ImGui::BeginPopupContextWindow(m_name.c_str(), m_button))
				{
					mvApp::GetApp()->pushParent(this);
					// set current menu value true
					m_value = true;

					// Context Menu
					if (getPopup() != "")
						ImGui::OpenPopup(getPopup().c_str());
				}
				else
					m_value = false;
			}

			else if (m_modal)
			{
				if (ImGui::BeginPopupModal(m_name.c_str()))
				{
					mvApp::GetApp()->pushParent(this);
					// set current menu value true
					m_value = true;

					// Context Menu
					if (getPopup() != "")
						ImGui::OpenPopup(getPopup().c_str());
				}
				else
					m_value = false;
			}

			else 
			{
				if (ImGui::BeginPopupContextItem(m_name.c_str(), m_button))
				{
					mvApp::GetApp()->pushParent(this);
					// set current menu value true
					m_value = true;

					// Context Menu
					if (getPopup() != "")
						ImGui::OpenPopup(getPopup().c_str());
				}
				else
					m_value = false;
			}
		}

		inline int getButton() const { return m_button; }

	private:

		bool m_modal;
		int  m_button;

	};

	//-----------------------------------------------------------------------------
	// mvEndPopup
	//-----------------------------------------------------------------------------
	class mvEndPopup : public mvNoneItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::EndPopup)

		mvEndPopup(const std::string& parent)
			: mvNoneItemBase(parent, "EndPopup")
		{
		}

		virtual void draw() override
		{
			mvApp::GetApp()->popParent();
			ImGui::EndPopup();
		}

	};

}