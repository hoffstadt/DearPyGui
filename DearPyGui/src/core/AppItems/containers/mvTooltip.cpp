#include "mvTooltip.h"
#include "mvApp.h"
#include "mvItemRegistry.h"

namespace Marvel {

	void mvTooltip::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID,
			"Adds an advanced tool tip for an item. This command must come immediately after the item the tip is for.", 
			{ "Containers", "Widgets" }, true);
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::UUID>("parent");
		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvTooltip::mvTooltip(mvUUID uuid)
		: 
		mvAppItem(uuid)
	{

		// has to be showed that way it can check for hovering
		// otherwise it will never show
		_show = true;
		
	}

	void mvTooltip::draw(ImDrawList* drawlist, float x, float y)
	{
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();

			for (auto& item : _children[1])
			{
				if (!item->preDraw())
					continue;

				item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

				item->postDraw();
			}

			ImGui::EndTooltip();
		}

	}

	void mvTooltip::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyRequiredArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				_parent = mvAppItem::GetIDFromPyObject(item);
				break;

			default:
				break;
			}
		}
	}

}