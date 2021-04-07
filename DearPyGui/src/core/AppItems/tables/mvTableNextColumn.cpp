#include "mvTableNextColumn.h"
#include "mvApp.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvTableNextColumn::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name", "", "'next_column'"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},

			}, "Changes to next column.", "None", "Containers") });
	}

	mvTableNextColumn::mvTableNextColumn(const std::string& name)
		: mvAppItem(name)
	{
	}

	void mvTableNextColumn::draw(ImDrawList* drawlist, float x, float y)
	{
		ImGui::TableNextColumn();
	}

	bool mvTableNextColumn::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvTable)
			return true;

		mvThrowPythonError(1000, "mvTableNextColumn parent must be a table.");
		MV_ITEM_REGISTRY_ERROR("mvTableNextColumn parent must be a table.");
		assert(false);
		return false;
	}

}