#include "mvKeyPressHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	mvKeyPressHandler::mvKeyPressHandler(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	void mvKeyPressHandler::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvKeyPressHandler*>(item);
		_key = titem->_key;
	}

	void mvKeyPressHandler::draw(ImDrawList* drawlist, float x, float y)
	{
		if (_key == -1)
		{
			for (int i = 0; i < IM_ARRAYSIZE(ImGui::GetIO().KeysDown); i++)
			{
				if (ImGui::IsKeyPressed(i))
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

		else if (ImGui::IsKeyPressed(_key))
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

	void mvKeyPressHandler::handleSpecificPositionalArgs(PyObject* dict)
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

	void mvKeyPressHandler::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "key")) _key = ToInt(item);
	}

	void mvKeyPressHandler::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "key", mvPyObject(ToPyInt(_key)));
	}

}