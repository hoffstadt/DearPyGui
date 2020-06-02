#include "mvInputText.h"
#include <imgui.h>
#include <string>
#include "Core/mvApp.h"

namespace Marvel {

	void mvInputText::draw()
	{

		if (m_hint == "")
		{
			if (ImGui::InputText(m_label.c_str(), (char*)m_value->c_str(), m_value->capacity() + 1))
				mvApp::GetApp()->triggerCallback(m_callback, m_name);
		}

		else
		{
			if (ImGui::InputTextWithHint(m_label.c_str(), m_hint.c_str(), (char*)m_value->c_str(), m_value->capacity() + 1))
				mvApp::GetApp()->triggerCallback(m_callback, m_name);
		}

		if(m_tip != "" && ImGui::IsItemHovered())
			ImGui::SetTooltip(m_tip.c_str());

	}

	PyObject* mvInputText::getPyValue()
	{

		PyObject* pvalue = Py_BuildValue("s", m_value->c_str());

		return pvalue;

	}

}