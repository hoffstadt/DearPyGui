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
				if(config.alias.empty())
					mvRunCallback(getCallback(false), uuid, ToPyInt(wheel), config.user_data);
				else
					mvRunCallback(getCallback(false), config.alias, ToPyInt(wheel), config.user_data);
			});
			
		}
	}
}