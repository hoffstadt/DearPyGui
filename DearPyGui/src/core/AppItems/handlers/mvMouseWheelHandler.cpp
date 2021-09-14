#include "mvMouseWheelHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	void mvMouseWheelHandler::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_CALLBACK)
		);
		args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "internal_dpg.mvReservedUUID_1", "Parent to add this item to. (runtime adding)" });
		
		mvPythonParserSetup setup;
		setup.about = "Adds a mouse wheel handler.";
		setup.category = { "Events", "Widgets" };
		setup.returnType = mvPyDataType::UUID;
		
		mvPythonParser parser = FinalizeParser(setup, args);

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
				if (_alias.empty())
					mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), _uuid, ToPyInt(wheel), _user_data);
				else
					mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), _alias, ToPyInt(wheel), _user_data);
			});
			
		}
	}
}