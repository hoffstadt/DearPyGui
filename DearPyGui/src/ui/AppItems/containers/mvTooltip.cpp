#include "mvTooltip.h"
#include "mvContext.h"
#include "mvItemRegistry.h"

namespace Marvel {

	mvTooltip::mvTooltip(mvUUID uuid)
		: 
		mvAppItem(uuid)
	{

		// has to be showed that way it can check for hovering
		// otherwise it will never show
		config.show = true;
		
	}

	void mvTooltip::draw(ImDrawList* drawlist, float x, float y)
	{
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();

			state.lastFrameUpdate = GContext->frame;
			state.visible = true;
			state.contextRegionAvail = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };
			state.rectSize = { ImGui::GetWindowSize().x, ImGui::GetWindowSize().y };

			for (auto& item : childslots[1])
				item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

			ImGui::EndTooltip();
		}

	}

	void mvTooltip::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				config.parent = GetIDFromPyObject(item);
				break;

			default:
				break;
			}
		}
	}

}