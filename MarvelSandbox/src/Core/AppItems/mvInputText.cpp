#include "mvInputText.h"
#include <imgui.h>
#include <string>

namespace Marvel {

	void mvInputText::draw()
	{

		if (m_hint == "")
		{
			if (ImGui::InputText(m_label.c_str(), (char*)m_value->c_str(), m_value->capacity() + 1))
				if (m_callback)
					m_callback(m_name, nullptr);
		}

		else
		{
			if (ImGui::InputTextWithHint(m_label.c_str(), m_hint.c_str(), (char*)m_value->c_str(), m_value->capacity() + 1))
				if (m_callback)
					m_callback(m_name, nullptr);
		}

		if(m_tip != "" && ImGui::IsItemHovered())
			ImGui::SetTooltip(m_tip.c_str());

	}

}