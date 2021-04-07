#include "mvUnindent.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"

namespace Marvel {


	void mvUnindent::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name", "", "'unindent'"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Float, "offset", "", "0.0"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Unindents following items.", "None", "Adding Widgets") });
	}


	mvUnindent::mvUnindent(const std::string& name)
		: mvFloatPtrBase(name)
	{
	}

	void mvUnindent::draw(ImDrawList* drawlist, float x, float y)
	{
		ImGui::Unindent(*m_value);
	}

}