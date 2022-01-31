#include "mvViewportMenuBar.h"
#include "mvContext.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "containers/mvWindowAppItem.h"
#include "containers/mvChild.h"
#include "mvPythonExceptions.h"

mvViewportMenuBar::mvViewportMenuBar(mvUUID uuid)
		: mvAppItem(uuid)
	{

		// TODO use code below to set item height when font and scale systems are done
		//float FontSize = ImGui::GetFontSize(); // = Base Font Size * Current Window Scale
		//ImGuiStyle currentStyle = ImGui::GetStyle(); // = Padding for the Top and Bottom
		//_height = int(currentStyle.FramePadding.y * 2 + FontSize);
		config.height = 21;
	}

void mvViewportMenuBar::draw(ImDrawList* drawlist, float x, float y)
{

	if (ImGui::BeginMainMenuBar())
	{

		for (auto& item : childslots[1])
			item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

		ImGui::EndMainMenuBar();
	}
}