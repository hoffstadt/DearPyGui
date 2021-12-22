#include "mvKeyDownHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	mvKeyDownHandler::mvKeyDownHandler(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	void mvKeyDownHandler::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvKeyDownHandler*>(item);
		_key = titem->_key;
	}

	void mvKeyDownHandler::draw(ImDrawList* drawlist, float x, float y)
	{

		if (_key == -1)
		{
			for (int i = 0; i < IM_ARRAYSIZE(ImGui::GetIO().KeysDown); i++)
			{
				if (ImGui::GetIO().KeysDown[i])
				{
					mvSubmitCallback([=]()
						{
							if(config.alias.empty())
								mvRunCallback(getCallback(false), uuid, ToPyMPair(i, ImGui::GetIO().KeysDownDuration[i]), config.user_data);
							else
								mvRunCallback(getCallback(false), config.alias, ToPyMPair(i, ImGui::GetIO().KeysDownDuration[i]), config.user_data);
						});
				}
			}
		}

		else if (ImGui::GetIO().KeysDown[_key])
		{
			mvSubmitCallback([=]()
				{
					if(config.alias.empty())
						mvRunCallback(getCallback(false), uuid, ToPyMPair(_key, ImGui::GetIO().KeysDownDuration[_key]), config.user_data);
					else
						mvRunCallback(getCallback(false), config.alias, ToPyMPair(_key, ImGui::GetIO().KeysDownDuration[_key]), config.user_data);
				});
		}
	}

	void mvKeyDownHandler::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
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

	void mvKeyDownHandler::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "key")) _key = ToInt(item);
	}

	void mvKeyDownHandler::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "key", mvPyObject(ToPyInt(_key)));
	}

}