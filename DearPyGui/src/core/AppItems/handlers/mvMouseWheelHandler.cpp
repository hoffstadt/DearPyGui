#include "mvMouseWheelHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	void mvMouseWheelHandler::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a handler which runs a given callback when the vertical mouse wheel is scrolled. Parent must be a handler registry.", { "Events", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_CALLBACK)
		);
		parser.addArg<mvPyDataType::UUID>("parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_1", "Parent to add this item to. (runtime adding)");
		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvMouseWheelHandler::mvMouseWheelHandler(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	void mvMouseWheelHandler::draw(ImDrawList* drawlist, float x, float y)
	{
		int wheel = (int)ImGui::GetIO().MouseWheel;
		if (wheel)
		{

			mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), _uuid, ToPyInt(wheel), _user_data);
			});
			
		}
	}
}