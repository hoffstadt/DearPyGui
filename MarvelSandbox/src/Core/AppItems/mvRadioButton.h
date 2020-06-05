#pragma once

#include "Core/AppItems/mvTypeBases.h"

namespace Marvel {

	class mvRadioButton : public mvIntItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::RadioButtons)

		mvRadioButton(const std::string& parent, const std::string& name, const std::vector<std::string>& itemnames, int default_value)
			: mvIntItemBase(parent, name, 1, default_value)
		{
			m_itemnames = itemnames;
		}

		virtual void draw() override
		{
			for (int i = 0; i < m_itemnames.size(); i++)
			{
				if (ImGui::RadioButton(m_itemnames[i].c_str(), &m_value[0], i))
					mvApp::GetApp()->triggerCallback(m_callback, m_name);
			}

			if (m_tip != "" && ImGui::IsItemHovered())
				ImGui::SetTooltip(m_tip.c_str());
		}

	private:

		std::vector<std::string> m_itemnames;

	};

}