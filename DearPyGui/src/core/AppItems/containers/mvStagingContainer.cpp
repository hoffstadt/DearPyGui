#include "mvStagingContainer.h"
#include "mvInput.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvStagingContainer::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		mvPythonParser parser(mvPyDataType::String, "Undocumented function", { "Containers", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID)
		);

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvStagingContainer::mvStagingContainer(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

}