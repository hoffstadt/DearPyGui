#include "mvItemHandlerRegistry.h"
#include "mvPythonExceptions.h"
#include "mvLog.h"

namespace Marvel {

	void mvItemHandlerRegistry::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_SHOW)
		);

		mvPythonParserSetup setup;
		setup.about = "Adds an item handler registry.";
		setup.category = { "Widgets", "Events", "Registries", "Containers"};
		setup.returnType = mvPyDataType::UUID;
		setup.createContextManager = true;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvItemHandlerRegistry::mvItemHandlerRegistry(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

	void mvItemHandlerRegistry::customAction(void* data)
	{

		for (auto& item : _children[1])
			item->customAction(data);

	}

	void mvItemHandlerRegistry::onBind(mvAppItem* item)
	{

		auto applicableState = GetApplicableState(item->getType());

		for (auto& handler : _children[1])
		{
			switch (handler->getType())
			{

			case mvAppItemType::mvActivatedHandler:
			{
				if(!(applicableState & MV_STATE_ACTIVATED))
					mvThrowPythonError(mvErrorCode::mvNone, "bind_item_handler_registry",
						"Item Handler Registry includes unapplicable handler: mvActivatedHandler", item);
				break;
			}

			case mvAppItemType::mvActiveHandler:
			{
				if (!(applicableState & MV_STATE_ACTIVE))
					mvThrowPythonError(mvErrorCode::mvNone, "bind_item_handler_registry",
						"Item Handler Registry includes unapplicable handler: mvActiveHandler", item);
				break;
			}

			case mvAppItemType::mvClickedHandler:
			{
				if (!(applicableState & MV_STATE_CLICKED))
					mvThrowPythonError(mvErrorCode::mvNone, "bind_item_handler_registry",
						"Item Handler Registry includes unapplicable handler: mvClickedHandler", item);
				break;
			}

			case mvAppItemType::mvDeactivatedAfterEditHandler:
			{
				if (!(applicableState & MV_STATE_DEACTIVATEDAE))
					mvThrowPythonError(mvErrorCode::mvNone, "bind_item_handler_registry",
						"Item Handler Registry includes unapplicable handler: mvDeactivatedAfterEditHandler", item);
				break;
			}

			case mvAppItemType::mvDeactivatedHandler:
			{
				if (!(applicableState & ~MV_STATE_DEACTIVATED))
					mvThrowPythonError(mvErrorCode::mvNone, "bind_item_handler_registry",
						"Item Handler Registry includes unapplicable handler: mvDeactivatedHandler", item);
				break;
			}

			case mvAppItemType::mvEditedHandler:
			{
				if (!(applicableState & ~MV_STATE_EDITED))
					mvThrowPythonError(mvErrorCode::mvNone, "bind_item_handler_registry",
						"Item Handler Registry includes unapplicable handler: mvEditedHandler", item);
				break;
			}

			case mvAppItemType::mvFocusHandler:
			{
				if (!(applicableState & ~MV_STATE_FOCUSED))
					mvThrowPythonError(mvErrorCode::mvNone, "bind_item_handler_registry",
						"Item Handler Registry includes unapplicable handler: mvFocusHandler", item);
				break;
			}

			case mvAppItemType::mvHoverHandler:
			{
				if (!(applicableState & ~MV_STATE_HOVER))
					mvThrowPythonError(mvErrorCode::mvNone, "bind_item_handler_registry",
						"Item Handler Registry includes unapplicable handler: mvHoverHandler", item);
				break;
			}

			case mvAppItemType::mvResizeHandler:
			{
				if (!(applicableState & ~MV_STATE_RECT_SIZE))
					mvThrowPythonError(mvErrorCode::mvNone, "bind_item_handler_registry",
						"Item Handler Registry includes unapplicable handler: mvResizeHandler", item);
				break;
			}

			case mvAppItemType::mvToggledOpenHandler:
			{
				if (!(applicableState & ~MV_STATE_TOGGLED_OPEN))
					mvThrowPythonError(mvErrorCode::mvNone, "bind_item_handler_registry",
						"Item Handler Registry includes unapplicable handler: mvToggledOpenHandler", item);
				break;
			}

			case mvAppItemType::mvVisibleHandler:
			{
				if (!(applicableState & MV_STATE_VISIBLE))
					mvThrowPythonError(mvErrorCode::mvNone, "bind_item_handler_registry",
						"Item Handler Registry includes unapplicable handler: mvVisibleHandler", item);
				break;
			}
			}
		}
	}
}