#include "mvRadioButton.h"
#include <imgui.h>
#include <string>
#include "Core/mvApp.h"

namespace Marvel {

	void mvRadioButton::draw()
	{

		for (int i = 0; i < m_itemnames.size(); i++)
		{
			if (ImGui::RadioButton(m_itemnames[i].c_str(), &m_value, i))
				mvApp::GetApp()->triggerCallback(m_callback, m_name);
		}

		if (m_tip != "" && ImGui::IsItemHovered())
			ImGui::SetTooltip(m_tip.c_str());

	}

	PyObject* mvRadioButton::getPyValue()
	{

		PyObject* pvalue = Py_BuildValue("i", m_value);

		return pvalue;

	}

}