#include "mvDrawing.h"
#include "mvApp.h"
#include "mvInput.h"
#include "mvItemRegistry.h"

namespace Marvel {

	void mvDrawing::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_drawing", mvPythonParser({
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
	}

	void mvDrawing::draw()
	{
		m_startx = (float)ImGui::GetCursorScreenPos().x;
		m_starty = (float)ImGui::GetCursorScreenPos().y;

		ImGui::PushClipRect({ m_startx, m_starty }, { m_startx + (float)m_core_config.width, m_starty + (float)m_core_config.height }, true);

		m_drawList->draw(ImGui::GetWindowDrawList(), m_startx, m_starty);

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

#ifndef MV_CPP

	PyObject* add_drawing(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* parent = "";
		const char* before = "";
		int width = 0;
		int height = 0;
		int show = true;
		float originx = 0.0f;
		float originy = 0.0f;
		float scalex = 1.0f;
		float scaley = 1.0f;

		if (!(*mvApp::GetApp()->getParsers())["add_drawing"].parse(args, kwargs, __FUNCTION__,
			&name, &parent, &before, &width, &height, &show, &originx, &originy, &scalex, &scaley))
			return ToPyBool(false);

		auto item = CreateRef<mvDrawing>(name);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		if (!item)
			return ToPyBool(false);

		mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before);

		return GetPyNone();
	}

#endif
}
