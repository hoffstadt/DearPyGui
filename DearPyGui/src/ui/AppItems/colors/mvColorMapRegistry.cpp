#include "mvColorMapRegistry.h"
#include "mvPythonExceptions.h"
#include "mvLog.h"

namespace Marvel {

	mvColorMapRegistry::mvColorMapRegistry(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
		config.show = false;
	}

	void mvColorMapRegistry::draw(ImDrawList* drawlist, float x, float y)
	{

		if (!config.show)
			return;

		ImGui::PushID(this);

		ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
		if (ImGui::Begin(info.internalLabel.c_str(), &config.show))
		{
			ImGui::Text("Builtin:");
			for (int i = 0; i < 16; i++)
				ImPlot::ColormapButton(ImPlot::GetColormapName(i), ImVec2(-1.0f, 0.0f), i);

			ImGui::Text("User:");

			for (auto& item : childslots[1])
				item->draw(drawlist, 0.0f, 0.0f);
			
		}

		ImGui::End();

		ImGui::PopID();

	}
}