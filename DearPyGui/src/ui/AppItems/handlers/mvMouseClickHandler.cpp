#include "mvMouseClickHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	mvMouseClickHandler::mvMouseClickHandler(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	void mvMouseClickHandler::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvMouseClickHandler*>(item);
		_button = titem->_button;
	}

	void mvMouseClickHandler::draw(ImDrawList* drawlist, float x, float y)
	{
		if (_button == -1)
		{
			for (int i = 0; i < IM_ARRAYSIZE(ImGui::GetIO().MouseDown); i++)
			{
				if (ImGui::IsMouseClicked(i))
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

		else if (ImGui::IsMouseClicked(_button))
		{
			mvSubmitCallback([=]()
				{
					if(config.alias.empty())
						mvRunCallback(getCallback(false), uuid, ToPyInt(_button), config.user_data);
					else
						mvRunCallback(getCallback(false), config.alias, ToPyInt(_button), config.user_data);
				});
		}
	}

	void mvMouseClickHandler::handleSpecificPositionalArgs(PyObject* dict)
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

	void mvMouseClickHandler::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "button")) _button = ToInt(item);
	}

	void mvMouseClickHandler::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "button", mvPyObject(ToPyInt(_button)));
	}

}