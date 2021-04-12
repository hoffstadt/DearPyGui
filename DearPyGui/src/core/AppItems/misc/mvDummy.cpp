#include "mvDummy.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"

namespace Marvel {



	void mvDummy::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{


		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("source");
		parser.removeArg("label");
		parser.removeArg("callback");
		parser.removeArg("callback_data");
		parser.removeArg("enabled");
		parser.finalize();

		parsers->insert({ s_command, parser });
	}


	mvDummy::mvDummy(const std::string& name)
		: mvAppItem(name)
	{
	}

	void mvDummy::draw(ImDrawList* drawlist, float x, float y)
	{
		ImGui::Dummy({ (float)m_width, (float)m_height });
	}

}