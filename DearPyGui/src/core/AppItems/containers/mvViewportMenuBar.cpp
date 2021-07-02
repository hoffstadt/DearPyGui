#include "mvViewportMenuBar.h"
#include "mvApp.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "containers/mvWindowAppItem.h"
#include "containers/mvChild.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvViewportMenuBar::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a menu bar to the viewport.", { "Containers", "Widgets" }, true);
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

	mvViewportMenuBar::mvViewportMenuBar(mvUUID uuid)
			: mvAppItem(uuid)
		{

			// TODO use code below to set item height when font and scale systems are done
			//float FontSize = ImGui::GetFontSize(); // = Base Font Size * Current Window Scale
			//ImGuiStyle currentStyle = ImGui::GetStyle(); // = Padding for the Top and Bottom
			//m_height = int(currentStyle.FramePadding.y * 2 + FontSize);
			m_height = 21;
		}

	void mvViewportMenuBar::draw(ImDrawList* drawlist, float x, float y)
	{

		if (ImGui::BeginMainMenuBar())
		{

			for (auto& item : m_children[1])
			{
				if (!item->preDraw())
					continue;

				item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

				item->postDraw();
			}
			ImGui::EndMainMenuBar();
		}
	}

}