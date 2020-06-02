#include "mvSameLine.h"
#include <imgui.h>
#include <string>
#include "Core/mvApp.h"

namespace Marvel {

	void mvSameLine::draw()
	{

		ImGui::SameLine(m_xoffset, m_spacing);

	}

}