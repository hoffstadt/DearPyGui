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
	if (_key == ImGuiKey_None)
	{
		for (int i = ImGuiKey_NamedKey_BEGIN; i < ImGuiKey_NamedKey_END; i++)
		{
			auto key = ImGui::GetKeyData(static_cast<ImGuiKey>(i));
			if (key->Down)
			{
				submitCallbackEx([=]() { return ToPyMPair(i, key->DownDuration); });
			}
		}
	}

	else if (ImGui::IsKeyDown(_key))
	{
		submitCallbackEx([=]() { return ToPyMPair(_key, ImGui::GetKeyData(_key)->DownDuration); });
	}
}

void mvKeyDownHandler::handleSpecificRequiredArgs(PyObject* dict)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	_key = (ImGuiKey)ToInt(PyTuple_GetItem(dict, 0));
}

void mvKeyDownHandler::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(dict, "key")) _key = (ImGuiKey)ToInt(item);
}

void mvKeyDownHandler::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;

	PyDict_SetItemString(dict, "key", mvPyObject(ToPyInt(_key)));
}

void mvKeyPressHandler::draw(ImDrawList* drawlist, float x, float y)
{
	if (_key == ImGuiKey_None)
	{
		for (int i = ImGuiKey_NamedKey_BEGIN; i < ImGuiKey_NamedKey_END; i++)
		{
			if (ImGui::IsKeyPressed(static_cast<ImGuiKey>(i)))
			{
				submitCallback(i);
			}
		}
	}

	else if (ImGui::IsKeyPressed(_key))
	{
		submitCallback(_key);
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
			_key = (ImGuiKey)ToInt(item);
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

	if (PyObject* item = PyDict_GetItemString(dict, "key")) _key = (ImGuiKey)ToInt(item);
}

void mvKeyPressHandler::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;

	PyDict_SetItemString(dict, "key", mvPyObject(ToPyInt(_key)));
}

void mvKeyReleaseHandler::draw(ImDrawList* drawlist, float x, float y)
{
	if (_key == ImGuiKey_None)
	{
		for (int i = ImGuiKey_NamedKey_BEGIN; i < ImGuiKey_NamedKey_END; i++)
		{
			if (ImGui::IsKeyReleased(static_cast<ImGuiKey>(i)))
			{
				submitCallback(i);
			}
		}
	}

	else if (ImGui::IsKeyReleased(_key))
	{
		submitCallback(_key);
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
			_key = (ImGuiKey)ToInt(item);
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

	if (PyObject* item = PyDict_GetItemString(dict, "key")) _key = (ImGuiKey)ToInt(item);
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
				submitCallback(i);
			}
		}
	}

	else if (ImGui::IsMouseClicked(_button))
	{
		submitCallback(_button);
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
				submitCallback(i);
			}
		}
	}

	else if (ImGui::IsMouseDoubleClicked(_button))
	{
		submitCallback(_button);
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
				submitCallbackEx([=]() { return ToPyMPair(i, ImGui::GetIO().MouseDownDuration[i]); });
			}
		}
	}

	else if (ImGui::GetIO().MouseDown[_button])
	{
		submitCallbackEx([=]() { return ToPyMPair(_button, ImGui::GetIO().MouseDownDuration[_button]); });
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
				submitCallbackEx([=]() { return ToPyMTrip(i, ImGui::GetMouseDragDelta(i).x, ImGui::GetMouseDragDelta(i).y); });
			}
		}
	}

	else if (ImGui::IsMouseDragging(_button, _threshold))
	{
		if (ImGui::IsMouseReleased(_button))
			ImGui::ResetMouseDragDelta(_button);

		submitCallbackEx([=]() { return ToPyMTrip(_button, ImGui::GetMouseDragDelta(_button).x, ImGui::GetMouseDragDelta(_button).y); });
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

			submitCallback(mousepos);
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
				submitCallback(i);
			}
		}
	}

	else if (ImGui::IsMouseReleased(_button))
	{
		submitCallback(_button);
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
		submitCallback(wheel);
	}
}