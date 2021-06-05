#include "mvViewportDrawlist.h"
#include "mvApp.h"
#include "mvInput.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"
#include "mvViewport.h"
#include "mvAppItemCommons.h"
#include "mvLog.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvViewportDrawlist::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String, "Undocumented function", { "Drawlist", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::Bool>("front", mvArgType::KEYWORD_ARG, "True");

		parser.finalize();
		parsers->insert({ s_command, parser });
	}

	mvViewportDrawlist::mvViewportDrawlist(const std::string& name)
		: mvAppItem(name)
	{
	}

	bool mvViewportDrawlist::canChildBeAdded(mvAppItemType type)
	{
		if (type == mvAppItemType::mvDrawLine) return true;
		if (type == mvAppItemType::mvDrawArrow) return true;
		if (type == mvAppItemType::mvDrawTriangle) return true;
		if (type == mvAppItemType::mvDrawCircle) return true;
		if (type == mvAppItemType::mvDrawBezierCurve) return true;
		if (type == mvAppItemType::mvDrawQuad) return true;
		if (type == mvAppItemType::mvDrawRect) return true;
		if (type == mvAppItemType::mvDrawText) return true;
		if (type == mvAppItemType::mvDrawPolygon) return true;
		if (type == mvAppItemType::mvDrawPolyline) return true;
		if (type == mvAppItemType::mvDrawImage) return true;
		if (type == mvAppItemType::mvDrawLayer) return true;

		mvThrowPythonError(1000, "Drawing children must be draw commands only.");
		MV_ITEM_REGISTRY_ERROR("Drawing children must be draw commands only.");
		assert(false);

		return false;
	}

	void mvViewportDrawlist::draw(ImDrawList* drawlist, float x, float y)
	{
		mvFontScope fscope(this);

		ImDrawList* internal_drawlist = m_front ? ImGui::GetForegroundDrawList() : ImGui::GetBackgroundDrawList();

		for (auto& item : m_children[2])
		{
			// skip item if it's not shown
			if (!item->m_show)
				continue;

			item->draw(internal_drawlist, 0.0f, 0.0f);

			item->getState().update();
		}

	}

	void mvViewportDrawlist::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "front")) m_front = ToBool(item);

	}

	void mvViewportDrawlist::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "front", ToPyBool(m_front));
	}

}
