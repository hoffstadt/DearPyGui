#pragma once

#include "Core/AppItems/mvTypeBases.h"

namespace Marvel {

	class mvCombo : public mvStringItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Combo)

		mvCombo(const std::string& parent, const std::string& name, const std::vector<std::string>& itemnames, const std::string& default_value)
			: mvStringItemBase(parent, name, default_value), m_names(itemnames)
		{}

		virtual void draw() override
		{

			if (ImGui::BeginCombo(m_label.c_str(), m_value.c_str())) // The second parameter is the label previewed before opening the combo.
			{
				for (int i = 0; i < m_names.size(); i++)
				{
					bool is_selected = (m_value == m_names[i]);
					if (ImGui::Selectable(m_names[i].c_str(), is_selected))
					{
						m_value = m_names[i];
						mvApp::GetApp()->triggerCallback(m_callback, m_name);
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
				}
				ImGui::EndCombo();
			}
		}

	private:

		std::vector<std::string> m_names;

	};

}