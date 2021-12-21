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
		ScopedID id(_uuid);

		if (_width != 0)
			ImGui::PushItemWidth((float)_width);


		ImGuiListClipper clipper;
		clipper.Begin((int)_children[1].size());

		while (clipper.Step())
		{
			for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
				_children[1][row_n]->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
		}
		clipper.End();
		if (_width != 0)
			ImGui::PopItemWidth();
	}

}