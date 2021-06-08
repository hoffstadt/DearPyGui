#include "mvDummy.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"

namespace Marvel {



	void mvDummy::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_WIDTH |
			MV_PARSER_ARG_HEIGHT |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_POS)
		);

		parser.finalize();

		parsers->insert({ s_command, parser });
	}


	mvDummy::mvDummy(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvDummy::draw(ImDrawList* drawlist, float x, float y)
	{
		ImGui::Dummy({ (float)m_width, (float)m_height });
	}

}