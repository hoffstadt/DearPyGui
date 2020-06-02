#include "mvGroup.h"
#include <imgui.h>
#include <string>
#include "Core/mvApp.h"

namespace Marvel {

	void mvGroup::draw()
	{
		mvApp::GetApp()->pushParent(this);
		ImGui::BeginGroup();

		if (m_tip != "" && ImGui::IsItemHovered())
			ImGui::SetTooltip(m_tip.c_str());

	}


	void mvEndGroup::draw()
	{
		mvApp::GetApp()->popParent();
		ImGui::EndGroup();
	}

}