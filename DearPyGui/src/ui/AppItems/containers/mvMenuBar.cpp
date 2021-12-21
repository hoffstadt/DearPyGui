#include "mvMenuBar.h"
#include "mvContext.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "containers/mvWindowAppItem.h"
#include "containers/mvChild.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	mvMenuBar::mvMenuBar(mvUUID uuid)
			: mvAppItem(uuid)
		{

			// TODO use code below to set item height when font and scale systems are done
			//float FontSize = ImGui::GetFontSize(); // = Base Font Size * Current Window Scale
			//ImGuiStyle currentStyle = ImGui::GetStyle(); // = Padding for the Top and Bottom
			//_height = int(currentStyle.FramePadding.y * 2 + FontSize);
			_height = 21;
		}

	void mvMenuBar::draw(ImDrawList* drawlist, float x, float y)
	{

		if (ImGui::BeginMenuBar())
		{

			_state.lastFrameUpdate = GContext->frame;
			_state.visible = true;
	
			for (auto& item : _children[1])
				item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

			ImGui::EndMenuBar();
		}

	}

}