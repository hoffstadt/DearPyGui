#include "mvSpacing.h"
#include "mvItemRegistry.h"

namespace Marvel {

	void mvSpacing::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_POS)
		);

		parser.addArg<mvPyDataType::Integer>("count", mvArgType::KEYWORD_ARG, "1", "Number of spacings to add");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}


	mvSpacing::mvSpacing(const std::string& name)
		: mvIntPtrBase(name)
	{
	}

	void mvSpacing::draw(ImDrawList* drawlist, float x, float y)
	{
		for (int i = 0; i < *m_value; i++)
			ImGui::Spacing();
	}

}