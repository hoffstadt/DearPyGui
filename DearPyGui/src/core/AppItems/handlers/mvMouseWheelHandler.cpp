#include "mvMouseWheelHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	void mvMouseWheelHandler::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Textures", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_CALLBACK |
			MV_PARSER_ARG_USER_DATA |
			MV_PARSER_ARG_PARENT)
		);
		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvMouseWheelHandler::mvMouseWheelHandler(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	bool mvMouseWheelHandler::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvStagingContainer) return true;
		if (type == mvAppItemType::mvHandlerRegistry) return true;

		mvThrowPythonError(1000, "Drawing item parent must be a drawing.");
		MV_ITEM_REGISTRY_ERROR("Drawing item parent must be a drawing.");
		assert(false);
		return false;
	}

	void mvMouseWheelHandler::draw(ImDrawList* drawlist, float x, float y)
	{

		if (ImGui::GetIO().MouseWheel != 0.0f)
		{

			mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
			{
				mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), m_uuid, ToPyInt((int)ImGui::GetIO().MouseWheel), m_user_data);
			});
			
		}
	}
}