#include "mvDrawlist.h"
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

	void mvDrawlist::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String, "Undocumented function", { "Drawlist", "Widgets" });
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("source");
		parser.removeArg("label");
		parser.removeArg("callback");
		parser.removeArg("callback_data");
		parser.removeArg("enabled");
		parser.removeArg("indent");
		parser.finalize();
		parsers->insert({ s_command, parser });
	}

	mvDrawlist::mvDrawlist(const std::string& name)
		: mvAppItem(name)
	{
	}

	bool mvDrawlist::canChildBeAdded(mvAppItemType type)
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

	void mvDrawlist::draw(ImDrawList* drawlist, float x, float y)
	{
		mvFontScope fscope(this);

		m_startx = (float)ImGui::GetCursorScreenPos().x;
		m_starty = (float)ImGui::GetCursorScreenPos().y;

		ImDrawList* internal_drawlist = ImGui::GetWindowDrawList();

		ImGui::PushClipRect({ m_startx, m_starty }, { m_startx + (float)m_width, m_starty + (float)m_height }, true);

		for (auto& item : m_children[2])
		{
			// skip item if it's not shown
			if (!item->m_show)
				continue;

			item->draw(internal_drawlist, m_startx, m_starty);

			item->getState().update();
		}

		ImGui::PopClipRect();
		ImGui::Dummy(ImVec2((float)m_width, (float)m_height));


		if (ImGui::IsItemHovered())
		{
			ImVec2 mousepos = ImGui::GetMousePos();
			mvInput::setDrawingMousePosition((float)mousepos.x- m_startx, (float)mousepos.y - m_starty);
		}
	}

}
