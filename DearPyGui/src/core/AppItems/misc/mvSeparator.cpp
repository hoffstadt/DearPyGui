#include "mvSeparator.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvImGuiThemeScope.h"

namespace Marvel {

	void mvSeparator::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name", "", "'separator'"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before","This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
		}, "Adds a horizontal line.", "None", "Adding Widgets") });
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