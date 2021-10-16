#include "mvSeparator.h"
#include "mvItemRegistry.h"

namespace Marvel {

	void mvSeparator::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_POS)
		);

		mvPythonParserSetup setup;
		setup.about = "Adds a horizontal line separator.";
		setup.category = { "Widgets" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}


	mvSeparator::mvSeparator(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvSeparator::draw(ImDrawList* drawlist, float x, float y)
	{

		ImGui::Separator();

	}

}