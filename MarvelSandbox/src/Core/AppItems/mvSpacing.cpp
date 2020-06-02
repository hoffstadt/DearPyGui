#include "mvSpacing.h"
#include <imgui.h>
#include <string>
#include "Core/mvApp.h"

namespace Marvel {

	void mvSpacing::draw()
	{

		for (int i = 0; i < m_value; i++)
			ImGui::Spacing();

	}



}