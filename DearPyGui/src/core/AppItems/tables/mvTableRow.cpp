#include "mvTableRow.h"
#include "mvApp.h"
#include "mvCore.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvTableRow::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Tables", "Containers", "Widgets" }, true);
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_HEIGHT |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_SHOW)
		);
		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvTableRow::mvTableRow(mvUUID uuid)
		: mvAppItem(uuid)
	{

	}

	bool mvTableRow::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvClipper) return true;
		if (type == mvAppItemType::mvStagingContainer) return true;
		if (type == mvAppItemType::mvFilterSet) return true;
		if (type == mvAppItemType::mvTable) return true;

		mvThrowPythonError(mvErrorCode::mvIncompatibleParent, s_command,
			"Incompatible parent. Acceptable parents include: table, clipper, filter set, staging container.", this);

		MV_ITEM_REGISTRY_ERROR("Node attribute parent must be node.");
		assert(false);
		return false;
	}

	void mvTableRow::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(m_uuid);

		if(m_location != 0)
			ImGui::TableNextRow(0, m_height);

		for (auto& item : m_children[1])
		{
			if (!item->preDraw())
				continue;

			ImGui::TableNextColumn();
			item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

			item->postDraw();
		}
	}

}