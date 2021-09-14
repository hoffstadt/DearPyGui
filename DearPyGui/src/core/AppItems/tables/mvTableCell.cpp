#include "mvTableCell.h"
#include "mvApp.h"
#include "mvCore.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvTableCell::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_HEIGHT |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_SHOW)
		);

		mvPythonParserSetup setup;
		setup.about = "Adds a table.";
		setup.category = { "Tables", "Containers", "Widgets" };
		setup.returnType = mvPyDataType::UUID;
		setup.createContextManager = true;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvTableCell::mvTableCell(mvUUID uuid)
		: mvAppItem(uuid)
	{

	}

	void mvTableCell::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(_uuid);

		for (auto& item : _children[1])
			item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
	}

}