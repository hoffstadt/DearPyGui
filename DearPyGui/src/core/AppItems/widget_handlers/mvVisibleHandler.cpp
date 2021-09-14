#include "mvVisibleHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	void mvVisibleHandler::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_CALLBACK)
		);

		mvPythonParserSetup setup;
		setup.about = "Adds a visible handler.";
		setup.category = { "Widgets", "Events" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvVisibleHandler::mvVisibleHandler(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	void mvVisibleHandler::customAction(void* data)
	{

		if (static_cast<mvAppItemState*>(data)->_visible)
		{
			mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
				{
					if (_alias.empty())
						mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), _uuid, GetPyNone(), _user_data);
					else
						mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), _alias, GetPyNone(), _user_data);
				});
		}
	}
}