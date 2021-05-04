#include "mvTooltip.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvTooltip::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String, "Undocumented function", { "Containers", "Widgets" });
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("source");
		parser.removeArg("width");
		parser.removeArg("height");
		parser.removeArg("indent");
		parser.removeArg("before");
		parser.removeArg("parent");
		parser.removeArg("label");
		parser.removeArg("callback");
		parser.removeArg("callback_data");
		parser.removeArg("enabled");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvTooltip::mvTooltip(const std::string& name)
		: 
		mvBoolPtrBase(name)
	{

		// has to be showed that way it can check for hovering
		// otherwise it will never show
		m_show = true;
		
	}

	void mvTooltip::draw(ImDrawList* drawlist, float x, float y)
	{
		if (ImGui::IsItemHovered())
		{
			mvImGuiThemeScope scope(this);
			mvFontScope fscope(this);
			ImGui::BeginTooltip();

			//we do this so that the children dont get the theme
			scope.cleanup();

			for (auto& item : m_children[1])
			{
				if (!item->preDraw())
					continue;

				item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

				item->postDraw();
			}

			ImGui::EndTooltip();
		}

	}

}