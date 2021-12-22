#include "mvMouseDownHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	mvMouseDownHandler::mvMouseDownHandler(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	void mvMouseDownHandler::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvMouseDownHandler*>(item);
		_button = titem->_button;
	}

	void mvMouseDownHandler::draw(ImDrawList* drawlist, float x, float y)
	{
		if (_button == -1)
		{
			for (int i = 0; i < IM_ARRAYSIZE(ImGui::GetIO().MouseDown); i++)
			{
				if (ImGui::GetIO().MouseDown[i])
				{
					mvSubmitCallback([=]()
						{
							if(config.alias.empty())
								mvRunCallback(getCallback(false), uuid, ToPyMPair(i, ImGui::GetIO().MouseDownDuration[i]), config.user_data);
							else
								mvRunCallback(getCallback(false), config.alias, ToPyMPair(i, ImGui::GetIO().MouseDownDuration[i]), config.user_data);
						});
				}
			}
		}

		else if (ImGui::GetIO().MouseDown[_button])
		{
			mvSubmitCallback([=]()
				{
					if(config.alias.empty())
						mvRunCallback(getCallback(false), uuid, ToPyMPair(_button, ImGui::GetIO().MouseDownDuration[_button]), config.user_data);
					else
						mvRunCallback(getCallback(false), config.alias, ToPyMPair(_button, ImGui::GetIO().MouseDownDuration[_button]), config.user_data);
				});
		}
	}

	void mvMouseDownHandler::handleSpecificPositionalArgs(PyObject* dict)
	{
		if (!VerifyPositionalArguments(GetParsers()[GetEntityCommand(type)], dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				_button = ToInt(item);
				break;

			default:
				break;
			}
		}
	}

	void mvMouseDownHandler::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "button")) _button = ToInt(item);
	}

	void mvMouseDownHandler::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "button", mvPyObject(ToPyInt(_button)));
	}

}