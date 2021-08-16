#include "mvTemplateRegistry.h"
#include "mvInput.h"
#include "mvApp.h"
#include "mvItemRegistry.h"

namespace Marvel {

	void mvTemplateRegistry::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Containers", "Widgets" }, true);
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID)
		);

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvTemplateRegistry::mvTemplateRegistry(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

}