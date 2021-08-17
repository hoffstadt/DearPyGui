#include "mvMouseMoveHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	void mvMouseMoveHandler::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a handler which runs a given callback when the mouse is moved. Parent must be a handler registry.", { "Events", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_CALLBACK)
		);
		parser.addArg<mvPyDataType::UUID>("parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_1", "Parent to add this item to. (runtime adding)");
		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvMouseMoveHandler::mvMouseMoveHandler(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

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

				mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
					{
						if(_alias.empty())
							mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), _uuid, ToPyPair(mousepos.x, mousepos.y), _user_data);
						else
							mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), _alias, ToPyPair(mousepos.x, mousepos.y), _user_data);
					});
			}
		}
	}
}