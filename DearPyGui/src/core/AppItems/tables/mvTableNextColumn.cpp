#include "mvTableNextColumn.h"
#include "mvApp.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvTableNextColumn::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Tables", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SHOW)
		);

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvTableNextColumn::mvTableNextColumn(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvTableNextColumn::draw(ImDrawList* drawlist, float x, float y)
	{
		ImGui::TableNextColumn();
	}

}