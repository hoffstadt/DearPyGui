#pragma once

#include "Core/AppItems/mvTypeBases.h"
#include "Core/mvDataStorage.h"
#include <misc/cpp/imgui_stdlib.h>
#include <utility>

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

		mvInputText(const std::string& parent, const std::string& name, const std::string& default_value,
              std::string  hint, bool multiline, ImGuiInputTextFlags flags)
			: mvStringItemBase(parent, name, default_value), m_hint(std::move(hint)), m_multiline(multiline), m_flags(flags)
		{
		}

		void draw() override
		{
			pushColorStyles();

			if (m_multiline)
				m_hint = "";

			if (m_hint.empty())
			{
				if(m_multiline)
				{
					if (ImGui::InputTextMultiline(m_label.c_str(), &m_value, ImVec2((float)m_width, (float)m_height), m_flags))
					{
						if (!m_dataSource.empty())
							mvDataStorage::AddData(m_dataSource, getPyValue());

						mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

						// Context Menu
						if (!getPopup().empty())
							ImGui::OpenPopup(getPopup().c_str());
					}
				}
				else
				{
					if (ImGui::InputText(m_label.c_str(), &m_value, m_flags))
					{
						if (!m_dataSource.empty())
							mvDataStorage::AddData(m_dataSource, getPyValue());

						mvApp::GetApp()->runCallback(m_callback, m_name);

						// Context Menu
						if (!getPopup().empty())
							ImGui::OpenPopup(getPopup().c_str());
					}
				}
			}

			else
			{
				if (ImGui::InputTextWithHint(m_label.c_str(), m_hint.c_str(), &m_value, m_flags))
				{
					if (!m_dataSource.empty())
						mvDataStorage::AddData(m_dataSource, getPyValue());

					mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

					// Context Menu
					if (!getPopup().empty())
						ImGui::OpenPopup(getPopup().c_str());
				}
			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			popColorStyles();
		}

	private:

		std::string         m_hint;
		bool                m_multiline;
		ImGuiInputTextFlags m_flags;

	};

}