#include "mvButton.h"
#include <imgui.h>
#include <string>
#include "Core/mvApp.h"

namespace Marvel {

	void mvButton::draw()
	{

		if (ImGui::Button(m_label.c_str()))
			mvApp::GetApp()->triggerCallback(m_callback, m_name);

	}


}