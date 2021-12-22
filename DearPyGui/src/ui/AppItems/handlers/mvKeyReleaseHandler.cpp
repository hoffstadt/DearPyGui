#include "mvKeyReleaseHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	mvKeyReleaseHandler::mvKeyReleaseHandler(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	void mvKeyReleaseHandler::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvKeyReleaseHandler*>(item);
		_key = titem->_key;
	}

	void mvKeyReleaseHandler::draw(ImDrawList* drawlist, float x, float y)
	{
		if (_key == -1)
		{
			for (int i = 0; i < IM_ARRAYSIZE(ImGui::GetIO().KeysDown); i++)
			{
				if (ImGui::GetIO().KeysDownDurationPrev[i] >= 0.0f && !ImGui::GetIO().KeysDown[i])
				{
					mvSubmitCallback([=]()
						{
							if(config.alias.empty())
								mvRunCallback(getCallback(false), uuid, ToPyInt(i), config.user_data);
							else
								mvRunCallback(getCallback(false), config.alias, ToPyInt(i), config.user_data);
						});
				}
			}
		}

		else if (ImGui::IsKeyReleased(_key))
		{
			mvSubmitCallback([=]()
				{
					if(config.alias.empty())
						mvRunCallback(getCallback(false), uuid, ToPyInt(_key), config.user_data);
					else
						mvRunCallback(getCallback(false), config.alias, ToPyInt(_key), config.user_data);
				});
		}
	}

	void mvKeyReleaseHandler::handleSpecificPositionalArgs(PyObject* dict)
	{
		if (!VerifyPositionalArguments(GetParsers()[GetEntityCommand(type)], dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				_key = ToInt(item);
				break;

			default:
				break;
			}
		}
	}

	void mvKeyReleaseHandler::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "key")) _key = ToInt(item);
	}

	void mvKeyReleaseHandler::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "key", mvPyObject(ToPyInt(_key)));
	}

}