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
					if (_alias.empty())
						mvRunCallback(getCallback(false), _uuid, GetPyNone(), _user_data);
					else
						mvRunCallback(getCallback(false), _alias, GetPyNone(), _user_data);
				});
		}
	}
}