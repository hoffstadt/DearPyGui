#include "mvGroup.h"
#include <imgui.h>
#include <string>
#include "Core/mvApp.h"

namespace Marvel {

	void mvGroup::draw()
	{
		if (m_width != 0)
			ImGui::PushItemWidth((float)m_width);

		mvApp::GetApp()->pushParent(this);
		ImGui::BeginGroup();

		if (m_tip != "" && ImGui::IsItemHovered())
			ImGui::SetTooltip(m_tip.c_str());

	}


	void mvEndGroup::draw()
	{
		if (m_width != 0)
			ImGui::PopItemWidth();

		mvApp::GetApp()->popParent();
		ImGui::EndGroup();
	}

}