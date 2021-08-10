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

	void mvTableRow::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(_uuid);

		if(_location != 0)
			ImGui::TableNextRow(0, _height);

		for (auto& item : _children[1])
		{
			if (!item->preDraw())
				continue;

			ImGui::TableNextColumn();
			item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

			item->postDraw();
		}
	}

}