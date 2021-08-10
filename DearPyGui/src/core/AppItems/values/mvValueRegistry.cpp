#include "mvValueRegistry.h"
#include "mvPythonExceptions.h"
#include "mvLog.h"

namespace Marvel {

	void mvValueRegistry::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Values", "Widgets" }, true);
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID)
		);

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvValueRegistry::mvValueRegistry(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

}