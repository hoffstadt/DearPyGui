#include "mvViewportDrawlist.h"
#include "mvApp.h"
#include "mvInput.h"
#include "mvItemRegistry.h"
#include "mvViewport.h"
#include "mvAppItemCommons.h"
#include "mvLog.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvViewportDrawlist::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "A container that is used to present draw items or layers directly to the viewport. By default this will draw to the back of teh viewport. Layers and draw items should be added to this widget as children.", { "Drawlist", "Containers", "Widgets"}, true);
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_SEARCH_DELAY |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::Bool>("front", mvArgType::KEYWORD_ARG, "True", "Draws to the front of the view port instead of the back.");

		parser.finalize();
		parsers->insert({ s_command, parser });
	}

	mvViewportDrawlist::mvViewportDrawlist(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvViewportDrawlist::draw(ImDrawList* drawlist, float x, float y)
	{

		ImDrawList* internal_drawlist = _front ? ImGui::GetForegroundDrawList() : ImGui::GetBackgroundDrawList();

		for (auto& item : _children[2])
		{
			// skip item if it's not shown
			if (!item->isShown())
				continue;

			item->draw(internal_drawlist, 0.0f, 0.0f);

			item->getState().update();
		}

	}

	void mvViewportDrawlist::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "front")) _front = ToBool(item);

	}

	void mvViewportDrawlist::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "front", ToPyBool(_front));
	}

}
