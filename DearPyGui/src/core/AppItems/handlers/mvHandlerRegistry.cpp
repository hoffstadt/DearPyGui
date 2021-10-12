#include "mvHandlerRegistry.h"
#include "mvPythonExceptions.h"
#include "mvLog.h"

namespace Marvel {

	void mvHandlerRegistry::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;
	
		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_SHOW)
		);

		mvPythonParserSetup setup;
		setup.about = "Adds a handler registry.";
		setup.category = { "Events", "Widgets", "Registries" };
		setup.returnType = mvPyDataType::UUID;
		setup.createContextManager = true;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvHandlerRegistry::mvHandlerRegistry(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

	void mvHandlerRegistry::draw(ImDrawList* drawlist, float x, float y)
	{

		for (auto& item : _children[1])
			item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

	}
}