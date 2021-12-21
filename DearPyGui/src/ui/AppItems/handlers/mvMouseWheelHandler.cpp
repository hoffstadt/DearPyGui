#include "mvMouseWheelHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	mvMouseWheelHandler::mvMouseWheelHandler(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	void mvMouseWheelHandler::draw(ImDrawList* drawlist, float x, float y)
	{
		int wheel = (int)ImGui::GetIO().MouseWheel;
		if (wheel)
		{

			mvSubmitCallback([=]()
			{
				if (_alias.empty())
					mvRunCallback(getCallback(false), _uuid, ToPyInt(wheel), _user_data);
				else
					mvRunCallback(getCallback(false), _alias, ToPyInt(wheel), _user_data);
			});
			
		}
	}
}