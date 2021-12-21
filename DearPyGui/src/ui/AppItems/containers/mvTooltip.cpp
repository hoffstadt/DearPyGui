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
		_show = true;
		
	}

	void mvTooltip::draw(ImDrawList* drawlist, float x, float y)
	{
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();

			_state.lastFrameUpdate = GContext->frame;
			_state.visible = true;
			_state.contextRegionAvail = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };
			_state.rectSize = { ImGui::GetWindowSize().x, ImGui::GetWindowSize().y };

			for (auto& item : _children[1])
				item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

			ImGui::EndTooltip();
		}

	}

	void mvTooltip::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(_type)], dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				_parent = GetIDFromPyObject(item);
				break;

			default:
				break;
			}
		}
	}

}