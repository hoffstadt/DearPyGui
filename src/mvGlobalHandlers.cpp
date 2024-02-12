#include "mvGlobalHandlers.h"
#include "mvPyUtils.h"
#include "mvUtilities.h"

void mvHandlerRegistry::draw(ImDrawList* drawlist, float x, float y)
{

	for (auto& item : childslots[1])
		item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

}

void mvKeyDownHandler::draw(ImDrawList* drawlist, float x, float y)
{

	if (_key == -1)
	{
		for (int i = 0; i < IM_ARRAYSIZE(ImGui::GetIO().KeysDown); i++)
		{
			if (ImGui::GetIO().KeysDown[i])
			{
				mvAddCallbackJob({*this, ToPyMPair(i, ImGui::GetIO().KeysDownDuration[i])}, false);
			}
		}
	}

	else if (ImGui::GetIO().KeysDown[_key])
	{
		mvAddCallbackJob({*this, ToPyMPair(_key, ImGui::GetIO().KeysDownDuration[_key])}, false);
	}
}

void mvKeyDownHandler::handleSpecificRequiredArgs(PyObject* dict)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	_key = ToInt(PyTuple_GetItem(dict, 0));
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

void mvKeyPressHandler::draw(ImDrawList* drawlist, float x, float y)
{
	if (_key == -1)
	{
		for (int i = 0; i < IM_ARRAYSIZE(ImGui::GetIO().KeysDown); i++)
		{
			if (ImGui::IsKeyPressed(i))
			{
				mvAddCallbackJob({*this, ToPyInt(i)}, false);
			}
		}
	}

	else if (ImGui::IsKeyPressed(_key))
	{
		mvAddCallbackJob({*this, ToPyInt(_key)}, false);
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

void mvKeyReleaseHandler::draw(ImDrawList* drawlist, float x, float y)
{
	if (_key == -1)
	{
		for (int i = 0; i < IM_ARRAYSIZE(ImGui::GetIO().KeysDown); i++)
		{
			if (ImGui::GetIO().KeysDownDurationPrev[i] >= 0.0f && !ImGui::GetIO().KeysDown[i])
			{
				mvAddCallbackJob({*this, ToPyInt(i)}, false);
			}
		}
	}

	else if (ImGui::IsKeyReleased(_key))
	{
		mvAddCallbackJob({*this, ToPyInt(_key)}, false);
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

void mvMouseClickHandler::draw(ImDrawList* drawlist, float x, float y)
{
	if (_button == -1)
	{
		for (int i = 0; i < IM_ARRAYSIZE(ImGui::GetIO().MouseDown); i++)
		{
			if (ImGui::IsMouseClicked(i))
			{
				mvAddCallbackJob({*this, ToPyInt(i)}, false);
			}
		}
	}

	else if (ImGui::IsMouseClicked(_button))
	{
		mvAddCallbackJob({*this, ToPyInt(_button)}, false);
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

void mvMouseDoubleClickHandler::draw(ImDrawList* drawlist, float x, float y)
{
	if (_button == -1)
	{
		for (int i = 0; i < IM_ARRAYSIZE(ImGui::GetIO().MouseDown); i++)
		{
			if (ImGui::IsMouseDoubleClicked(i))
			{
				mvAddCallbackJob({*this, ToPyInt(i)}, false);
			}
		}
	}

	else if (ImGui::IsMouseDoubleClicked(_button))
	{
		mvAddCallbackJob({*this, ToPyInt(_button)}, false);
	}
}

void mvMouseDoubleClickHandler::handleSpecificPositionalArgs(PyObject* dict)
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

void mvMouseDoubleClickHandler::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(dict, "button")) _button = ToInt(item);
}

void mvMouseDoubleClickHandler::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;

	PyDict_SetItemString(dict, "button", mvPyObject(ToPyInt(_button)));
}

void mvMouseDownHandler::draw(ImDrawList* drawlist, float x, float y)
{
	if (_button == -1)
	{
		for (int i = 0; i < IM_ARRAYSIZE(ImGui::GetIO().MouseDown); i++)
		{
			if (ImGui::GetIO().MouseDown[i])
			{
				mvAddCallbackJob({*this, ToPyMPair(i, ImGui::GetIO().MouseDownDuration[i])}, false);
			}
		}
	}

	else if (ImGui::GetIO().MouseDown[_button])
	{
		mvAddCallbackJob({*this, ToPyMPair(_button, ImGui::GetIO().MouseDownDuration[_button])}, false);
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
				mvAddCallbackJob({*this, ToPyMTrip(i, ImGui::GetMouseDragDelta(i).x, ImGui::GetMouseDragDelta(i).y)}, false);
			}
		}
	}

	else if (ImGui::IsMouseDragging(_button, _threshold))
	{
		if (ImGui::IsMouseReleased(_button))
			ImGui::ResetMouseDragDelta(_button);

		mvAddCallbackJob({*this, ToPyMTrip(_button, ImGui::GetMouseDragDelta(_button).x, ImGui::GetMouseDragDelta(_button).y)}, false);
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


void mvMouseMoveHandler::draw(ImDrawList* drawlist, float x, float y)
{

	// update mouse
	// mouse move event
	ImVec2 mousepos = ImGui::GetMousePos();
	if (ImGui::IsMousePosValid(&mousepos))
	{
		if (_oldPos.x != mousepos.x || _oldPos.y != mousepos.y)
		{
			_oldPos = mousepos;

			mvAddCallbackJob({*this, ToPyPair(mousepos.x, mousepos.y)}, false);
		}
	}
}

void mvMouseReleaseHandler::draw(ImDrawList* drawlist, float x, float y)
{
	if (_button == -1)
	{
		for (int i = 0; i < IM_ARRAYSIZE(ImGui::GetIO().MouseDown); i++)
		{
			if (ImGui::IsMouseReleased(i))
			{
				mvAddCallbackJob({*this, ToPyInt(i)}, false);
			}
		}
	}

	else if (ImGui::IsMouseReleased(_button))
	{
		mvAddCallbackJob({*this, ToPyInt(_button)}, false);
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

void mvMouseWheelHandler::draw(ImDrawList* drawlist, float x, float y)
{
	int wheel = (int)ImGui::GetIO().MouseWheel;
	if (wheel)
	{
		mvAddCallbackJob({*this, ToPyInt(wheel)}, false);
	}
}
