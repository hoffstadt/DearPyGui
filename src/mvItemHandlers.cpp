#include "mvItemHandlers.h"
#include "mvPyUtils.h"

void mvItemHandlerRegistry::checkEvents(void* data)
{

	for (auto& item : childslots[1])
	{
		if (item->config.show)
			item->customAction(data);
	}

}

void mvItemHandlerRegistry::onBind(mvAppItem* item)
{

	auto applicableState = DearPyGui::GetApplicableState(item->type);

	for (auto& handler : childslots[1])
	{
		switch (handler->type)
		{

		case mvAppItemType::mvActivatedHandler:
		{
			if (!(applicableState & MV_STATE_ACTIVATED))
				mvThrowPythonError(mvErrorCode::mvNone, "bind_item_handler_registry",
					"Item Handler Registry includes inapplicable handler: mvActivatedHandler", item);
			break;
		}

		case mvAppItemType::mvActiveHandler:
		{
			if (!(applicableState & MV_STATE_ACTIVE))
				mvThrowPythonError(mvErrorCode::mvNone, "bind_item_handler_registry",
					"Item Handler Registry includes inapplicable handler: mvActiveHandler", item);
			break;
		}

		case mvAppItemType::mvClickedHandler:
		{
			if (!(applicableState & MV_STATE_CLICKED))
				mvThrowPythonError(mvErrorCode::mvNone, "bind_item_handler_registry",
					"Item Handler Registry includes inapplicable handler: mvClickedHandler", item);
			break;
		}

		case mvAppItemType::mvDoubleClickedHandler:
		{
			// if an item can be clicked, it can be double clicked as well
			if (!(applicableState & MV_STATE_CLICKED))
				mvThrowPythonError(mvErrorCode::mvNone, "bind_item_handler_registry",
					"Item Handler Registry includes inapplicable handler: mvDoubleClickedHandler", item);
			break;
		}

		case mvAppItemType::mvDeactivatedAfterEditHandler:
		{
			if (!(applicableState & MV_STATE_DEACTIVATEDAE))
				mvThrowPythonError(mvErrorCode::mvNone, "bind_item_handler_registry",
					"Item Handler Registry includes inapplicable handler: mvDeactivatedAfterEditHandler", item);
			break;
		}

		case mvAppItemType::mvDeactivatedHandler:
		{
			if (!(applicableState & MV_STATE_DEACTIVATED))
				mvThrowPythonError(mvErrorCode::mvNone, "bind_item_handler_registry",
					"Item Handler Registry includes inapplicable handler: mvDeactivatedHandler", item);
			break;
		}

		case mvAppItemType::mvEditedHandler:
		{
			if (!(applicableState & MV_STATE_EDITED))
				mvThrowPythonError(mvErrorCode::mvNone, "bind_item_handler_registry",
					"Item Handler Registry includes inapplicable handler: mvEditedHandler", item);
			break;
		}

		case mvAppItemType::mvFocusHandler:
		{
			if (!(applicableState & MV_STATE_FOCUSED))
				mvThrowPythonError(mvErrorCode::mvNone, "bind_item_handler_registry",
					"Item Handler Registry includes inapplicable handler: mvFocusHandler", item);
			break;
		}

		case mvAppItemType::mvHoverHandler:
		{
			if (!(applicableState & MV_STATE_HOVER))
				mvThrowPythonError(mvErrorCode::mvNone, "bind_item_handler_registry",
					"Item Handler Registry includes inapplicable handler: mvHoverHandler", item);
			break;
		}

		case mvAppItemType::mvResizeHandler:
		{
			if (!(applicableState & MV_STATE_RECT_SIZE))
				mvThrowPythonError(mvErrorCode::mvNone, "bind_item_handler_registry",
					"Item Handler Registry includes inapplicable handler: mvResizeHandler", item);
			break;
		}

		case mvAppItemType::mvToggledOpenHandler:
		{
			if (!(applicableState & MV_STATE_TOGGLED_OPEN))
				mvThrowPythonError(mvErrorCode::mvNone, "bind_item_handler_registry",
					"Item Handler Registry includes inapplicable handler: mvToggledOpenHandler", item);
			break;
		}

		case mvAppItemType::mvVisibleHandler:
		{
			if (!(applicableState & MV_STATE_VISIBLE))
				mvThrowPythonError(mvErrorCode::mvNone, "bind_item_handler_registry",
					"Item Handler Registry includes inapplicable handler: mvVisibleHandler", item);
			break;
		}

            default: break;
		}
	}
}

void mvActivatedHandler::customAction(void* data)
{

	mvAppItemState* state = static_cast<mvAppItemState*>(data);
	if (state->activated)
	{
		mvAddCallbackJob({*this, ToPyUUID(state->parent)}, false);
	}
}

void mvActiveHandler::customAction(void* data)
{

	mvAppItemState* state = static_cast<mvAppItemState*>(data);
	if (state->active)
	{
		mvAddCallbackJob({*this, ToPyUUID(state->parent)}, false);
	}
}

void mvClickedHandler::customAction(void* data)
{

	mvAppItemState* state = static_cast<mvAppItemState*>(data);
	if (_button == -1 || _button == 0)
		if (state->leftclicked)
		{
			mvPyObject pArgs(PyTuple_New(2));
			PyTuple_SetItem(pArgs, 0, ToPyInt(0));
			PyTuple_SetItem(pArgs, 1, ToPyUUID(state->parent)); // steals data, so don't deref
			mvAddCallbackJob({*this, pArgs}, false);
		}

	if (_button == -1 || _button == 1)
		if (state->rightclicked)
		{
			mvPyObject pArgs(PyTuple_New(2));
			PyTuple_SetItem(pArgs, 0, ToPyInt(1));
			PyTuple_SetItem(pArgs, 1, ToPyUUID(state->parent)); // steals data, so don't deref
			mvAddCallbackJob({*this, pArgs}, false);
		}

	if (_button == -1 || _button == 2)
		if (state->middleclicked)
		{
			mvPyObject pArgs(PyTuple_New(2));
			PyTuple_SetItem(pArgs, 0, ToPyInt(2));
			PyTuple_SetItem(pArgs, 1, ToPyUUID(state->parent)); // steals data, so don't deref
			mvAddCallbackJob({*this, pArgs}, false);
		}

}

void mvClickedHandler::handleSpecificRequiredArgs(PyObject* dict)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	_button = ToInt(PyTuple_GetItem(dict, 0));
}

void mvClickedHandler::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(dict, "button")) _button = ToInt(item);
}

void mvClickedHandler::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;

	PyDict_SetItemString(dict, "button", ToPyInt(_button));
}

void mvDoubleClickedHandler::customAction(void* data)
{
	mvAppItemState* state = static_cast<mvAppItemState*>(data);

	int i = (_button < 0)? 0 : _button ;
	int end = (_button < 0)? state->doubleclicked.size() : (i + 1);

	for (; i < end; i++)
	{
		if (state->doubleclicked[i])
		{
			mvPyObject pArgs(PyTuple_New(2));
			PyTuple_SetItem(pArgs, 0, ToPyInt(i));
			PyTuple_SetItem(pArgs, 1, ToPyUUID(state->parent)); // steals data, so don't deref
			mvAddCallbackJob({*this, pArgs}, false);
		}
	}
}

void mvDoubleClickedHandler::handleSpecificRequiredArgs(PyObject* dict)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	_button = ToInt(PyTuple_GetItem(dict, 0));
}

void mvDoubleClickedHandler::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(dict, "button")) _button = ToInt(item);
}

void mvDoubleClickedHandler::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;

	PyDict_SetItemString(dict, "button", ToPyInt(_button));
}

void mvDeactivatedAfterEditHandler::customAction(void* data)
{

	mvAppItemState* state = static_cast<mvAppItemState*>(data);
	if (state->deactivatedAfterEdit)
	{
		mvAddCallbackJob({*this, ToPyUUID(state->parent)}, false);
	}
}

void mvDeactivatedHandler::customAction(void* data)
{
	mvAppItemState* state = static_cast<mvAppItemState*>(data);
	if (state->deactivated)
	{
		mvAddCallbackJob({*this, ToPyUUID(state->parent)}, false);
	}
}

void mvEditedHandler::customAction(void* data)
{

	mvAppItemState* state = static_cast<mvAppItemState*>(data);
	if (state->edited)
	{
		mvAddCallbackJob({*this, ToPyUUID(state->parent)}, false);
	}
}

void mvFocusHandler::customAction(void* data)
{

	mvAppItemState* state = static_cast<mvAppItemState*>(data);
	if (state->focused)
	{
		mvAddCallbackJob({*this, ToPyUUID(state->parent)}, false);
	}
}

void mvHoverHandler::customAction(void* data)
{
	mvAppItemState* state = static_cast<mvAppItemState*>(data);
	if (state->hovered)
	{
		mvAddCallbackJob({*this, ToPyUUID(state->parent)}, false);
	}
}

void mvResizeHandler::customAction(void* data)
{
	mvAppItemState* state = static_cast<mvAppItemState*>(data);
	if (state->mvRectSizeResized)
	{
		mvAddCallbackJob({*this, ToPyUUID(state->parent)}, false);
	}
}

void mvToggledOpenHandler::customAction(void* data)
{
	if (static_cast<mvAppItemState*>(data)->toggledOpen)
	{
		mvAddCallbackJob({*this, nullptr}, false);
	}
}

void mvVisibleHandler::customAction(void* data)
{
	if (static_cast<mvAppItemState*>(data)->visible)
	{
		mvAddCallbackJob({*this, nullptr}, false);
	}
}
