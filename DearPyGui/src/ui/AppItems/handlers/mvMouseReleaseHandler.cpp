#include "mvMouseReleaseHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

mvMouseReleaseHandler::mvMouseReleaseHandler(mvUUID uuid)
	:
	mvAppItem(uuid)
{

}

void mvMouseReleaseHandler::applySpecificTemplate(mvAppItem* item)
{
	auto titem = static_cast<mvMouseReleaseHandler*>(item);
	_button = titem->_button;
}

void mvMouseReleaseHandler::draw(ImDrawList* drawlist, float x, float y)
{
	if (_button == -1)
	{
		for (int i = 0; i < IM_ARRAYSIZE(ImGui::GetIO().MouseDown); i++)
		{
			if (ImGui::IsMouseReleased(i))
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

	else if (ImGui::IsMouseReleased(_button))
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

void mvMouseReleaseHandler::handleSpecificPositionalArgs(PyObject* dict)
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

void mvMouseReleaseHandler::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(dict, "button")) _button = ToInt(item);
}

void mvMouseReleaseHandler::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;

	PyDict_SetItemString(dict, "button", mvPyObject(ToPyInt(_button)));
}