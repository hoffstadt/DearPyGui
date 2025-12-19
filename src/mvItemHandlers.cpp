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

		case mvAppItemType::mvScrollHandler:
		{
			if (!(applicableState & ~MV_STATE_SCROLL))
				mvThrowPythonError(mvErrorCode::mvNone, "bind_item_handler_registry",
					"Item Handler Registry includes inapplicable handler: mvScrollHandler", item);
			break;
		}

            default: break;
		}
	}
}

void mvItemHandler::submitHandler(mvAppItem* parent)
{
	submitCallbackEx([uuid=parent->uuid, alias=parent->config.alias] () {
		return ToPyUUID(uuid, alias);
	});
}

void mvBoolStateHandler::checkEvent(bool curState, bool prevState, mvAppItem* parent)
{
	mvEventType eventType = curState?
			(prevState? mvEventType_On : mvEventType_EnterAndOn) :
			(prevState? mvEventType_LeaveAndOff : mvEventType_Off);

	if (trackedEventType & eventType)
	{
		// We do not pass eventType to callback yet in order to keep it compatible
		// with the old version.
		submitHandler(parent);
	}
}

void mvBoolStateHandler::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(dict, "event_type"))
	{
		if (item != Py_None)
			trackedEventType = static_cast<mvEventType>(ToInt(item));
	}
}

void mvBoolStateHandler::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;

	PyDict_SetItemString(dict, "event_type", mvPyObject(ToPyInt(trackedEventType)));
}

void mvActivatedHandler::customAction(void* data)
{

	mvAppItemState* state = static_cast<mvAppItemState*>(data);
	if (state->activated)
	{
		submitHandler(state->parent);
	}
}

void mvActiveHandler::customAction(void* data)
{

	mvAppItemState* state = static_cast<mvAppItemState*>(data);
	if (state->active)
	{
		submitHandler(state->parent);
	}
}

void mvClickedHandler::customAction(void* data)
{

	mvAppItemState* state = static_cast<mvAppItemState*>(data);

	b8 clicked[] = {state->leftclicked, state->rightclicked, state->middleclicked};

	int i = (_button < 0)? 0 : _button ;
	int end = (_button < 0)? (int)std::size(clicked) : (i + 1);

	for (; i < end; i++)
	{
		if (clicked[i])
		{
			mvAppItem* parent = state->parent;
			submitCallbackEx([i, uuid=parent->uuid, alias=parent->config.alias] () {
				PyObject* app_data = PyTuple_New(2);
				PyTuple_SetItem(app_data, 0, ToPyInt(i));
				PyTuple_SetItem(app_data, 1, ToPyUUID(uuid, alias));
				return app_data;
			});
		}
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
			mvAppItem* parent = state->parent;
			submitCallbackEx([i, uuid=parent->uuid, alias=parent->config.alias] () {
				PyObject* app_data = PyTuple_New(2);
				PyTuple_SetItem(app_data, 0, ToPyInt(i));
				PyTuple_SetItem(app_data, 1, ToPyUUID(uuid, alias));
				return app_data;
			});
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
		submitHandler(state->parent);
	}
}

void mvDeactivatedHandler::customAction(void* data)
{
	mvAppItemState* state = static_cast<mvAppItemState*>(data);
	if (state->deactivated)
	{
		submitHandler(state->parent);
	}
}

void mvEditedHandler::customAction(void* data)
{

	mvAppItemState* state = static_cast<mvAppItemState*>(data);
	if (state->edited)
	{
		submitHandler(state->parent);
	}
}

void mvFocusHandler::customAction(void* data)
{
	mvAppItemState* state = static_cast<mvAppItemState*>(data);
	checkEvent(state->focused, state->prevFocused, state->parent);
}

void mvHoverHandler::customAction(void* data)
{
	mvAppItemState* state = static_cast<mvAppItemState*>(data);
	checkEvent(state->hovered, state->prevHovered, state->parent);
}

void mvResizeHandler::customAction(void* data)
{
	mvAppItemState* state = static_cast<mvAppItemState*>(data);
	if (state->mvRectSizeResized)
	{
		submitHandler(state->parent);
	}
}

void mvToggledOpenHandler::customAction(void* data)
{
	mvAppItemState* state = static_cast<mvAppItemState*>(data);
	if (state->toggledOpen)
	{
		submitHandler(state->parent);
	}
}

void mvVisibleHandler::customAction(void* data)
{
	mvAppItemState* state = static_cast<mvAppItemState*>(data);
	if (static_cast<mvAppItemState*>(data)->visible)
	{
		submitHandler(state->parent);
	}
}

PyObject* mvScrollHandler::makeAppData(mvUUID uuid, const std::string& alias, mvScrollDirection dir, bool isScrolling, float pos, float max)
{
	PyObject* app_data = PyTuple_New(5);
	PyTuple_SetItem(app_data, 0, ToPyUUID(uuid, alias));
	PyTuple_SetItem(app_data, 1, ToPyInt(dir));
	PyTuple_SetItem(app_data, 2, ToPyFloat(pos));
	PyTuple_SetItem(app_data, 3, ToPyFloat(max));
	PyTuple_SetItem(app_data, 4, ToPyBool(isScrolling));
	return app_data;
}

void mvScrollHandler::customAction(void* data)
{
	mvAppItemState* state = static_cast<mvAppItemState*>(data);
	mvAppItem* parent = state->parent;
	if (state->scrolledX)
	{
		submitCallbackEx([=,
						  uuid=parent->uuid,
						  alias=parent->config.alias,
						  isScrolling=state->isScrollingX,
						  pos=state->scrollPos.x,
						  max=state->scrollMax.x] ()
		{
			return makeAppData(uuid, alias, mvScrollDirection_XAxis, isScrolling, pos, max);
		});
	}

	if (state->scrolledY)
	{
		submitCallbackEx([=,
						  uuid=parent->uuid,
						  alias=parent->config.alias,
						  isScrolling=state->isScrollingY,
						  pos=state->scrollPos.y,
						  max=state->scrollMax.y] ()
		{
			return makeAppData(uuid, alias, mvScrollDirection_YAxis, isScrolling, pos, max);
		});
	}
}
