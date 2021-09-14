#include "mvClipper.h"
#include "mvInput.h"
#include "mvApp.h"
#include "mvItemRegistry.h"

namespace Marvel {

	void mvClipper::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_WIDTH |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SEARCH_DELAY |
			MV_PARSER_ARG_SHOW)
		);

		mvPythonParserSetup setup;
		setup.about = "Helper to manually clip large list of items. Increases performance by not searching or drawing widgets outside of the clipped region.";
		setup.category = { "Containers", "Widgets" };
		setup.returnType = mvPyDataType::UUID;
		setup.createContextManager = true;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvClipper::mvClipper(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvClipper::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(_uuid);

		if (_width != 0)
			ImGui::PushItemWidth((float)_width);


		ImGuiListClipper clipper;
		clipper.Begin(_children[1].size());

		while (clipper.Step())
		{
			for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
				_children[1][row_n]->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
		}
		clipper.End();
		if (_width != 0)
			ImGui::PopItemWidth();
	}

}