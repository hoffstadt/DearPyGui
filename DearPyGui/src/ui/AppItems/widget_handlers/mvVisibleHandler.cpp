#include "mvVisibleHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	mvVisibleHandler::mvVisibleHandler(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	void mvVisibleHandler::customAction(void* data)
	{

		if (static_cast<mvAppItemState*>(data)->visible)
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