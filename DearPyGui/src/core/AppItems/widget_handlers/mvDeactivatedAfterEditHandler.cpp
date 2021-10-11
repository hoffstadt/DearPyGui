#include "mvDeactivatedAfterEditHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	void mvDeactivatedAfterEditHandler::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_CALLBACK)
		);

		mvPythonParserSetup setup;
		setup.about = "Adds a deactivated after edit handler.";
		setup.category = { "Widgets", "Events" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvDeactivatedAfterEditHandler::mvDeactivatedAfterEditHandler(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	void mvDeactivatedAfterEditHandler::customAction(void* data)
	{

		if (static_cast<mvAppItemState*>(data)->deactivatedAfterEdit)
		{
			mvSubmitCallback([=]()
				{
					if (_alias.empty())
						mvRunCallback(getCallback(false), _uuid, GetPyNone(), _user_data);
					else
						mvRunCallback(getCallback(false), _alias, GetPyNone(), _user_data);
				});
		}
	}
}