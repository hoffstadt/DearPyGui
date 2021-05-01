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
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("source");
		parser.removeArg("label");
		parser.removeArg("callback");
		parser.removeArg("callback_data");
		parser.removeArg("enabled");
		parser.removeArg("width");
		parser.removeArg("height");
		parser.removeArg("parent");
		parser.removeArg("before");
		parser.removeArg("show");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvStagingContainer::mvStagingContainer(const std::string& name)
		: mvAppItem(name)
	{
	}

}