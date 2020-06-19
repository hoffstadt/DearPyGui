#pragma once

#include "Core/AppItems/mvTypeBases.h"
#include <misc/cpp/imgui_stdlib.h>

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvInputText
//
//-----------------------------------------------------------------------------

namespace Marvel {

	class mvInputText : public mvStringItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::InputText)

		mvInputText(const std::string& parent, const std::string& name, const std::string& hint, bool multiline, ImGuiInputTextFlags flags)
			: mvStringItemBase(parent, name, ""), m_hint(hint), m_multiline(multiline), m_flags(flags)
		{
		}

		virtual void draw() override
		{
			if (m_multiline)
				m_hint = "";

			if (m_hint == "")
			{
				if(m_multiline)
				{
					if (ImGui::InputTextMultiline(m_label.c_str(), &m_value, ImVec2(m_width, m_height)))
					{
						mvApp::GetApp()->triggerCallback(m_callback, m_name);

						// Context Menu
						if (getPopup() != "")
							ImGui::OpenPopup(getPopup().c_str());
					}
				}
				else
				{
					if (ImGui::InputText(m_label.c_str(), &m_value, m_flags))
					{
						mvApp::GetApp()->triggerCallback(m_callback, m_name);

						// Context Menu
						if (getPopup() != "")
							ImGui::OpenPopup(getPopup().c_str());
					}
				}
			}

			else
			{
				if (ImGui::InputTextWithHint(m_label.c_str(), m_hint.c_str(), &m_value, m_flags))
				{
					mvApp::GetApp()->triggerCallback(m_callback, m_name);

					// Context Menu
					if (getPopup() != "")
						ImGui::OpenPopup(getPopup().c_str());
				}
			}
		}

	private:

		std::string         m_hint;
		bool                m_multiline;
		ImGuiInputTextFlags m_flags;

	};

}