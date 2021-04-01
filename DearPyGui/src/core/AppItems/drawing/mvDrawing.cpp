#include "mvDrawing.h"
#include "mvApp.h"
#include "mvInput.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"
#include "mvDrawList.h"
#include "mvDrawCmd.h"
#include "mvWindow.h"
#include "mvAppItems.h"
#include "mvLog.h"

namespace Marvel {

	void mvDrawing::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ s_parser, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Integer, "width","", "0"},
			{mvPythonDataType::Integer, "height","", "0"},
			{mvPythonDataType::Bool, "show","Attempt to render", "True"},
		}, "Adds a drawing widget.", "None", "Drawing") });

	}

	mvDrawing::mvDrawing(const std::string& name)
		: mvAppItem(name)
	{
		m_drawList = CreateRef<mvDrawList>();
		m_description.container = true;
	}

	bool mvDrawing::canChildBeAdded(mvAppItemType type)
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

		mvThrowPythonError(1000, "Drawing children must be draw commands only.");
		MV_ITEM_REGISTRY_ERROR("Drawing children must be draw commands only.");
		assert(false);

		return false;
	}

	void mvDrawing::draw(ImDrawList* drawlist, float x, float y)
	{
		mvFontScope fscope(this);

		m_startx = (float)ImGui::GetCursorScreenPos().x;
		m_starty = (float)ImGui::GetCursorScreenPos().y;

		ImGui::PushClipRect({ m_startx, m_starty }, { m_startx + (float)m_core_config.width, m_starty + (float)m_core_config.height }, true);

		for (auto& item : m_children0)
		{
			// skip item if it's not shown
			if (!item->m_core_config.show)
				continue;

			item->draw(drawlist, m_startx, m_starty);

			item->getState().update();
		}

		ImGui::PopClipRect();
		ImGui::Dummy(ImVec2((float)m_core_config.width, (float)m_core_config.height));


		if (ImGui::IsItemHovered())
		{
			ImVec2 mousepos = ImGui::GetMousePos();
			mvInput::setDrawingMousePosition((float)mousepos.x- m_startx, (float)mousepos.y - m_starty);
		}
	}

	mvRef<mvDrawList> mvDrawing::getDrawList()
	{
		return m_drawList;
	}

	PyObject* mvDrawing::add_drawing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		static int i = 0; i++;
		std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
		const char* name = sname.c_str();
		const char* parent = "";
		const char* before = "";
		int width = 0;
		int height = 0;
		int show = true;
		float originx = 0.0f;
		float originy = 0.0f;
		float scalex = 1.0f;
		float scaley = 1.0f;

		if (!(mvApp::GetApp()->getParsers())["add_drawing"].parse(args, kwargs, __FUNCTION__,
			&name, &parent, &before, &width, &height, &show, &originx, &originy, &scalex, &scaley))
			return ToPyBool(false);

		auto item = CreateRef<mvDrawing>(name);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		if (!item)
			return ToPyBool(false);

		if (mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before))
		{
			mvApp::GetApp()->getItemRegistry().pushParent(item);
			if (!show)
				item->hide();
		}

		return ToPyString(name);
	}

}
