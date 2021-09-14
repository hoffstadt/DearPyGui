#include "mvValueRegistry.h"
#include "mvPythonExceptions.h"
#include "mvLog.h"

namespace Marvel {

	void mvValueRegistry::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID)
		);

		mvPythonParserSetup setup;
		setup.about = "Adds a value registry.";
		setup.category = { "Widgets", "Values", "Containers"};
		setup.returnType = mvPyDataType::UUID;
		setup.createContextManager = true;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvValueRegistry::mvValueRegistry(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

}