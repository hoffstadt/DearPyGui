#include "mvClipper.h"
#include "mvContext.h"
#include "mvItemRegistry.h"

namespace Marvel {

	mvClipper::mvClipper(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvClipper::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(uuid);

		if (config.width != 0)
			ImGui::PushItemWidth((float)config.width);


		ImGuiListClipper clipper;
		clipper.Begin((int)childslots[1].size());

		while (clipper.Step())
		{
			for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
				childslots[1][row_n]->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
		}
		clipper.End();
		if (config.width != 0)
			ImGui::PopItemWidth();
	}

}