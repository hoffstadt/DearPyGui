#include "mvColorMapRegistry.h"
#include "mvPythonExceptions.h"
#include "mvLog.h"

namespace Marvel {

	mvColorMapRegistry::mvColorMapRegistry(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
		_show = false;
	}

	void mvColorMapRegistry::draw(ImDrawList* drawlist, float x, float y)
	{

		if (!_show)
			return;

		ImGui::PushID(this);

		ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
		if (ImGui::Begin(_internalLabel.c_str(), &_show))
		{
			ImGui::Text("Builtin:");
			for (int i = 0; i < 16; i++)
				ImPlot::ColormapButton(ImPlot::GetColormapName(i), ImVec2(-1.0f, 0.0f), i);

			ImGui::Text("User:");

			for (auto& item : _children[1])
				item->draw(drawlist, 0.0f, 0.0f);
			
		}

		ImGui::End();

		ImGui::PopID();

	}
}