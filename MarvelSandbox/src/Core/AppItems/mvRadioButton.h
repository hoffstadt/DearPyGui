#pragma once

#include "Core/mvAppItem.h"

namespace Marvel {

	class mvRadioButton : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::RadioButtons)

		mvRadioButton(const std::string& parent, const std::string& name, const std::vector<std::string>& itemnames, int default_value)
			: mvAppItem(parent, name)
		{
			m_value = default_value;
			m_itemnames = itemnames;
		}

		virtual PyObject* getPyValue() override
		{
			PyObject* pvalue = Py_BuildValue("i", m_value);

			return pvalue;
		}

		virtual void draw() override
		{
			for (int i = 0; i < m_itemnames.size(); i++)
			{
				if (ImGui::RadioButton(m_itemnames[i].c_str(), &m_value, i))
					mvApp::GetApp()->triggerCallback(m_callback, m_name);
			}

			if (m_tip != "" && ImGui::IsItemHovered())
				ImGui::SetTooltip(m_tip.c_str());
		}

		inline int getValue() const { return m_value; }

	private:

		int m_value;
		std::vector<std::string> m_itemnames;

	};

}