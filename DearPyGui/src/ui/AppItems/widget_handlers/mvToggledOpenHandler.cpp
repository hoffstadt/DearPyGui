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
					if (_alias.empty())
						mvRunCallback(getCallback(false), _uuid, GetPyNone(), _user_data);
					else
						mvRunCallback(getCallback(false), _alias, GetPyNone(), _user_data);
				});
		}
	}

}