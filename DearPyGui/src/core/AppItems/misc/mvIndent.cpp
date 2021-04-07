#include "mvIndent.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"

namespace Marvel {


	void mvIndent::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name", "", "'indent'"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Float, "offset", "", "0.0"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Adds an indent to following items. Must be closed with the unindent command.", "None", "Adding Widgets") });
	}


	mvIndent::mvIndent(const std::string& name)
		: mvFloatPtrBase(name)
	{
	}

	void mvIndent::draw(ImDrawList* drawlist, float x, float y)
	{
		ImGui::Indent(*m_value);
	}

}