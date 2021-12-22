#include "mvMouseMoveHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	mvMouseMoveHandler::mvMouseMoveHandler(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	void mvMouseMoveHandler::draw(ImDrawList* drawlist, float x, float y)
	{

		// update mouse
		// mouse move event
		ImVec2 mousepos = ImGui::GetMousePos();
		if (ImGui::IsMousePosValid(&mousepos))
		{
			if (_oldPos.x != mousepos.x || _oldPos.y != mousepos.y)
			{
				_oldPos = mousepos;

				mvSubmitCallback([=]()
					{
						if(config.alias.empty())
							mvRunCallback(getCallback(false), uuid, ToPyPair(mousepos.x, mousepos.y), config.user_data);
						else
							mvRunCallback(getCallback(false), config.alias, ToPyPair(mousepos.x, mousepos.y), config.user_data);
					});
			}
		}
	}
}