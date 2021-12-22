#include "mvItemHandlerRegistry.h"
#include "mvPythonExceptions.h"
#include "mvLog.h"

namespace Marvel {

	mvItemHandlerRegistry::mvItemHandlerRegistry(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

	void mvItemHandlerRegistry::checkEvents(void* data)
	{

		for (auto& item : childslots[1])
		{
			if(item->config.show)
				item->customAction(data);
		}

	}

	void mvItemHandlerRegistry::onBind(mvAppItem* item)
	{

		auto applicableState = GetApplicableState(item->type);

		for (auto& handler : childslots[1])
		{
			switch (handler->type)
			{

			case mvAppItemType::mvActivatedHandler:
			{
				if(!(applicableState & MV_STATE_ACTIVATED))
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

			case mvAppItemType::mvDeactivatedAfterEditHandler:
			{
				if (!(applicableState & MV_STATE_DEACTIVATEDAE))
					mvThrowPythonError(mvErrorCode::mvNone, "bind_item_handler_registry",
						"Item Handler Registry includes inapplicable handler: mvDeactivatedAfterEditHandler", item);
				break;
			}

			case mvAppItemType::mvDeactivatedHandler:
			{
				if (!(applicableState & ~MV_STATE_DEACTIVATED))
					mvThrowPythonError(mvErrorCode::mvNone, "bind_item_handler_registry",
						"Item Handler Registry includes inapplicable handler: mvDeactivatedHandler", item);
				break;
			}

			case mvAppItemType::mvEditedHandler:
			{
				if (!(applicableState & ~MV_STATE_EDITED))
					mvThrowPythonError(mvErrorCode::mvNone, "bind_item_handler_registry",
						"Item Handler Registry includes inapplicable handler: mvEditedHandler", item);
				break;
			}

			case mvAppItemType::mvFocusHandler:
			{
				if (!(applicableState & ~MV_STATE_FOCUSED))
					mvThrowPythonError(mvErrorCode::mvNone, "bind_item_handler_registry",
						"Item Handler Registry includes inapplicable handler: mvFocusHandler", item);
				break;
			}

			case mvAppItemType::mvHoverHandler:
			{
				if (!(applicableState & ~MV_STATE_HOVER))
					mvThrowPythonError(mvErrorCode::mvNone, "bind_item_handler_registry",
						"Item Handler Registry includes inapplicable handler: mvHoverHandler", item);
				break;
			}

			case mvAppItemType::mvResizeHandler:
			{
				if (!(applicableState & ~MV_STATE_RECT_SIZE))
					mvThrowPythonError(mvErrorCode::mvNone, "bind_item_handler_registry",
						"Item Handler Registry includes inapplicable handler: mvResizeHandler", item);
				break;
			}

			case mvAppItemType::mvToggledOpenHandler:
			{
				if (!(applicableState & ~MV_STATE_TOGGLED_OPEN))
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
			}
		}
	}
}