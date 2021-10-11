#include "mvMouseMoveHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	void mvMouseMoveHandler::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_CALLBACK)
		);
		args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_1", "Parent to add this item to. (runtime adding)" });
		
		mvPythonParserSetup setup;
		setup.about = "Adds a mouse move handler.";
		setup.category = { "Events", "Widgets" };
		setup.returnType = mvPyDataType::UUID;
		
		mvPythonParser parser = FinalizeParser(setup, args);

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

				mvSubmitCallback([=]()
					{
						if(_alias.empty())
							mvRunCallback(getCallback(false), _uuid, ToPyPair(mousepos.x, mousepos.y), _user_data);
						else
							mvRunCallback(getCallback(false), _alias, ToPyPair(mousepos.x, mousepos.y), _user_data);
					});
			}
		}
	}
}