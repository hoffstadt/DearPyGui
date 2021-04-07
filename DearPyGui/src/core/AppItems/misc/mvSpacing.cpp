#include "mvSpacing.h"
#include "mvItemRegistry.h"

namespace Marvel {

	void mvSpacing::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name", "", "'spacing'"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "count", "", "1"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Adds vertical spacing.", "None", "Adding Widgets") });
	}


	mvSpacing::mvSpacing(const std::string& name)
		: mvIntPtrBase(name)
	{
	}

	void mvSpacing::draw(ImDrawList* drawlist, float x, float y)
	{
		for (int i = 0; i < *m_value; i++)
			ImGui::Spacing();
	}

}