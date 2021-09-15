#include "mvTooltip.h"
#include "mvContext.h"
#include "mvItemRegistry.h"

namespace Marvel {

	void mvTooltip::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_SHOW)
		);

		args.push_back({ mvPyDataType::UUID, "parent" });

		mvPythonParserSetup setup;
		setup.about = "Adds a tooltip window.";
		setup.category = { "Containers", "Widgets" };
		setup.returnType = mvPyDataType::UUID;
		setup.createContextManager = true;

		mvPythonParser parser = FinalizeParser(setup, args);

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
				item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

			ImGui::EndTooltip();
		}

	}

	void mvTooltip::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!VerifyRequiredArguments(GetParsers()[s_command], dict))
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