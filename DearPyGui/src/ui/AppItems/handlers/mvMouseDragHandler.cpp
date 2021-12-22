#include "mvMouseDragHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	mvMouseDragHandler::mvMouseDragHandler(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	void mvMouseDragHandler::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvMouseDragHandler*>(item);
		_button = titem->_button;
		_threshold = titem->_threshold;
	}

	void mvMouseDragHandler::draw(ImDrawList* drawlist, float x, float y)
	{
		if (_button == -1)
		{
			for (int i = 0; i < IM_ARRAYSIZE(ImGui::GetIO().MouseDown); i++)
			{
				if (ImGui::IsMouseReleased(i))
					ImGui::ResetMouseDragDelta(i);

				if (ImGui::IsMouseDragging(i, _threshold))
				{
					mvSubmitCallback([=]()
						{
							if(config.alias.empty())
								mvRunCallback(getCallback(false), uuid,
									ToPyMTrip(i, ImGui::GetMouseDragDelta(i).x, ImGui::GetMouseDragDelta(i).y), config.user_data);
							else
								mvRunCallback(getCallback(false), config.alias,
									ToPyMTrip(i, ImGui::GetMouseDragDelta(i).x, ImGui::GetMouseDragDelta(i).y), config.user_data);
						});
				}
			}
		}

		else if (ImGui::IsMouseDragging(_button, _threshold))
		{
			if (ImGui::IsMouseReleased(_button))
				ImGui::ResetMouseDragDelta(_button);
			mvSubmitCallback([=]()
				{
					if(config.alias.empty())
						mvRunCallback(getCallback(false), uuid,
							ToPyMTrip(_button, ImGui::GetMouseDragDelta(_button).x, ImGui::GetMouseDragDelta(_button).y), config.user_data);
					else
						mvRunCallback(getCallback(false), config.alias,
							ToPyMTrip(_button, ImGui::GetMouseDragDelta(_button).x, ImGui::GetMouseDragDelta(_button).y), config.user_data);
				});
		}
	}

	void mvMouseDragHandler::handleSpecificPositionalArgs(PyObject* dict)
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
			case 1:
				_threshold = ToFloat(item);
				break;

			default:
				break;
			}
		}
	}

	void mvMouseDragHandler::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "button")) _button = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "threshold")) _threshold = ToFloat(item);
	}

	void mvMouseDragHandler::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "button", mvPyObject(ToPyInt(_button)));
		PyDict_SetItemString(dict, "threshold", mvPyObject(ToPyFloat(_threshold)));
	}
}