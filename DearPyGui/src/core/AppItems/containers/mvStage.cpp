#include "mvStage.h"
#include "mvInput.h"
#include "mvApp.h"
#include "mvItemRegistry.h"

namespace Marvel {

	void mvStage::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID)
		);

		mvPythonParserSetup setup;
		setup.about = "Adds a stage.";
		setup.category = { "Containers", "Widgets"};
		setup.returnType = mvPyDataType::UUID;
		setup.createContextManager = true;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvStage::mvStage(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

}