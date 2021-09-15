#include "mvTemplateRegistry.h"
#include "mvContext.h"
#include "mvItemRegistry.h"

namespace Marvel {

	void mvTemplateRegistry::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID)
		);

		mvPythonParserSetup setup;
		setup.about = "Adds a template registry.";
		setup.category = { "Containers", "Widgets", "Registries" };
		setup.returnType = mvPyDataType::UUID;
		setup.createContextManager = true;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvTemplateRegistry::mvTemplateRegistry(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

}