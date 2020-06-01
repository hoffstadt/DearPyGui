#include "mvInputText.h"
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

namespace Marvel {

	void mvInputText::draw()
	{

		if (m_hint == "")
			ImGui::InputText(m_props.label.c_str(), &m_value);

		else
			
			ImGui::InputTextWithHint(m_props.label.c_str(), m_hint.c_str(), &m_value);

	}

}