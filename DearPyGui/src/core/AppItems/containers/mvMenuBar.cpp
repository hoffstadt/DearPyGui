#include "mvMenuBar.h"
#include "mvApp.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "containers/mvWindowAppItem.h"
#include "containers/mvChild.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvMenuBar::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a menu bar to a window.", { "Containers", "Widgets" }, true);
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_SEARCH_DELAY |
			MV_PARSER_ARG_SHOW)
		);
		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvMenuBar::mvMenuBar(mvUUID uuid)
			: mvAppItem(uuid)
		{

			// TODO use code below to set item height when font and scale systems are done
			//float FontSize = ImGui::GetFontSize(); // = Base Font Size * Current Window Scale
			//ImGuiStyle currentStyle = ImGui::GetStyle(); // = Padding for the Top and Bottom
			//_height = int(currentStyle.FramePadding.y * 2 + FontSize);
			_height = 21;
		}

	void mvMenuBar::draw(ImDrawList* drawlist, float x, float y)
	{

		if (ImGui::BeginMenuBar())
		{
			_state.update();

			for (auto& item : _children[1])
				item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

			ImGui::EndMenuBar();
		}
	}

}