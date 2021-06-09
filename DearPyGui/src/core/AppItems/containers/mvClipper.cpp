#include "mvClipper.h"
#include "mvInput.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvClipper::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Containers", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_WIDTH |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SEARCH_DELAY |
			MV_PARSER_ARG_SHOW)
		);

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvClipper::mvClipper(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvClipper::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(m_uuid);
		mvFontScope fscope(this);

		if (m_width != 0)
			ImGui::PushItemWidth((float)m_width);


		ImGuiListClipper clipper;
		clipper.Begin(m_children[1].size());

		while (clipper.Step())
		{
			for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
			{
				if (!m_children[1][row_n]->preDraw())
					continue;
				m_children[1][row_n]->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
				m_children[1][row_n]->postDraw();
			}
		}
		clipper.End();
		if (m_width != 0)
			ImGui::PopItemWidth();
	}

}