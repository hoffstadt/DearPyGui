#include "mvSeparator.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvImGuiThemeScope.h"

namespace Marvel {

	void mvSeparator::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("source");
		parser.removeArg("width");
		parser.removeArg("height");
		parser.removeArg("label");
		parser.removeArg("callback");
		parser.removeArg("callback_data");
		parser.removeArg("enabled");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}


	mvSeparator::mvSeparator(const std::string& name)
		: mvAppItem(name)
	{
	}

	void mvSeparator::draw(ImDrawList* drawlist, float x, float y)
	{
		mvImGuiThemeScope scope(this);

		ImGui::Separator();

	}

}