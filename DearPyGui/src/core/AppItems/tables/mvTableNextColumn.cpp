#include "mvTableNextColumn.h"
#include "mvApp.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvTableNextColumn::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String, "Undocumented function", { "Tables", "Widgets" });
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