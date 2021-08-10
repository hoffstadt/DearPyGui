#include "mvFontRegistry.h"
#include "mvFont.h"
#include "mvPythonExceptions.h"
#include "mvLog.h"

namespace Marvel {

	void mvFontRegistry::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Fonts", "Containers" }, true);
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_SHOW)
		);

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvFontRegistry::mvFontRegistry(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
		_show = true;
	}

	void mvFontRegistry::draw(ImDrawList* drawlist, float x, float y)
	{
		//ImGuiIO& io = ImGui::GetIO();
		//io.Fonts->Clear();
		//io.FontDefault = io.Fonts->AddFontDefault();

		for (auto& item : _children[1])
		{
			item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
		}
		_show = false;
	}

	void mvFontRegistry::customAction()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->Clear();
		io.FontDefault = io.Fonts->AddFontDefault();

		for (auto& item : _children[1])
		{
			item->customAction();
		}
	}

}