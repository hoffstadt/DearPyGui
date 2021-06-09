#include "mvSeparator.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvImGuiThemeScope.h"

namespace Marvel {

	void mvSeparator::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID);
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_POS)
		);

		parser.finalize();

		parsers->insert({ s_command, parser });
	}


	mvSeparator::mvSeparator(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvSeparator::draw(ImDrawList* drawlist, float x, float y)
	{
		mvImGuiThemeScope scope(this);

		ImGui::Separator();

	}

}