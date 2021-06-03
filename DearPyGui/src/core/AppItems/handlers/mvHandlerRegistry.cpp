#include "mvHandlerRegistry.h"
#include "mvPythonExceptions.h"
#include "mvLog.h"

namespace Marvel {

	void mvHandlerRegistry::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String, "Undocumented function", { "Textures", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_SHOW)
		);

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvHandlerRegistry::mvHandlerRegistry(const std::string& name)
		:
		mvAppItem(name)
	{
	}

	void mvHandlerRegistry::draw(ImDrawList* drawlist, float x, float y)
	{

		for (auto& item : m_children[3])
		{
			if (!item->preDraw())
				continue;

			item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
		}

	}

	bool mvHandlerRegistry::canChildBeAdded(mvAppItemType type)
	{
		if (type == mvAppItemType::mvKeyDownHandler) return true;
		if (type == mvAppItemType::mvKeyPressHandler) return true;
		if (type == mvAppItemType::mvKeyReleaseHandler) return true;
		if (type == mvAppItemType::mvMouseMoveHandler) return true;
		if (type == mvAppItemType::mvMouseWheelHandler) return true;
		if (type == mvAppItemType::mvMouseClickHandler) return true;
		if (type == mvAppItemType::mvMouseDoubleClickHandler) return true;
		if (type == mvAppItemType::mvMouseDownHandler) return true;
		if (type == mvAppItemType::mvMouseReleaseHandler) return true;
		if (type == mvAppItemType::mvMouseDragHandler) return true;

		mvThrowPythonError(1000, "Drawing children must be draw commands only.");
		MV_ITEM_REGISTRY_ERROR("Drawing children must be draw commands only.");
		assert(false);

		return false;
	}
}