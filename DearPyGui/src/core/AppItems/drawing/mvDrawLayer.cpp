#include "mvDrawLayer.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvDrawLayer::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Creates a layer that can be drawn to. Useful for grouping drawing items.", { "Drawlist", "Widgets" }, true);
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SHOW)
		);

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvDrawLayer::mvDrawLayer(mvUUID uuid)
		:
		mvAppItem(uuid)
	{

	}

	void mvDrawLayer::draw(ImDrawList* drawlist, float x, float y)
	{
		for (auto& item : _children[2])
		{
			// skip item if it's not shown
			if (!item->_show)
				continue;

			item->draw(drawlist, x, y);

			item->getState().update();
		}
	}

}