#include "mvMouseMoveHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

	void mvMouseMoveHandler::InsertParser(std::map<std::string, mvPythonParser>* parsers)
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

	mvMouseMoveHandler::mvMouseMoveHandler(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	bool mvMouseMoveHandler::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvStagingContainer) return true;
		if (type == mvAppItemType::mvHandlerRegistry) return true;

		mvThrowPythonError(1000, "Drawing item parent must be a drawing.");
		MV_ITEM_REGISTRY_ERROR("Drawing item parent must be a drawing.");
		assert(false);
		return false;
	}

	void mvMouseMoveHandler::draw(ImDrawList* drawlist, float x, float y)
	{

		// update mouse
		// mouse move event
		ImVec2 mousepos = ImGui::GetMousePos();
		if (ImGui::IsMousePosValid(&mousepos))
		{
			if (m_oldPos.x != mousepos.x || m_oldPos.y != mousepos.y)
			{
				m_oldPos = mousepos;

				mvApp::GetApp()->getCallbackRegistry().submitCallback([=]()
					{
						mvApp::GetApp()->getCallbackRegistry().runCallback(getCallback(false), m_uuid, ToPyPair(mousepos.x, mousepos.y), m_user_data);
					});
			}
		}
	}
}