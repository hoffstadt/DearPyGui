#include "mvDummy.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"

namespace Marvel {



	void mvDummy::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name", "", "'dummy'"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "width", "", "0"},
			{mvPythonDataType::Integer, "height", "", "0"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"}
		}, "Adds a spacer or 'dummy' object.", "None", "Adding Widgets") });
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