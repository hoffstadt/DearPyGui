#include "mvActivatedHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	void mvActivatedHandler::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a handler which runs a given callback when the specified item is activated.", { "Events", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_CALLBACK)
		);

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvActivatedHandler::mvActivatedHandler(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	void mvActivatedHandler::draw(ImDrawList* drawlist, float x, float y)
	{

		if (ImGui::IsItemActivated())
		{
			mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
				{
					if(_alias.empty())
						mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), _uuid, GetPyNone(), _user_data);
					else
						mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), _alias, GetPyNone(), _user_data);
				});
		}
	}

}