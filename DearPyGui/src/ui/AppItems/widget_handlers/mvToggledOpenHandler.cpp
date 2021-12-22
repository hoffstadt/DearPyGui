#include "mvToggledOpenHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	mvToggledOpenHandler::mvToggledOpenHandler(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	void mvToggledOpenHandler::customAction(void* data)
	{

		if (static_cast<mvAppItemState*>(data)->toggledOpen)
		{
			mvSubmitCallback([=]()
				{
					if(config.alias.empty())
						mvRunCallback(getCallback(false), uuid, GetPyNone(), config.user_data);
					else
						mvRunCallback(getCallback(false), config.alias, GetPyNone(), config.user_data);
				});
		}
	}

}