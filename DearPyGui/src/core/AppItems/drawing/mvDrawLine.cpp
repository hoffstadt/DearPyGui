#include "mvDrawLine.h"
#include "mvLog.h"
#include "mvItemRegistry.h"

namespace Marvel {

	void mvDrawLine::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		parsers->insert({ s_parser, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::FloatList, "p1"},
			{mvPythonDataType::FloatList, "p2"},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::Integer, "thickness"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name", "", "''"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Draws a line on a drawing.", "None", "Drawing") });
	}

	mvDrawLine::mvDrawLine(const std::string& name, const mvVec2& p1, const mvVec2& p2, const mvColor& color, float thickness)
		:
		mvAppItem(name),
		m_p1(p1),
		m_p2(p2),
		m_color(color),
		m_thickness(thickness)
	{
		m_description.target = 0;
	}

	bool mvDrawLine::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvDrawing) return true;
		if (type == mvAppItemType::mvWindowAppItem) return true;

		mvThrowPythonError(1000, "Drawing item parent must be a drawing.");
		MV_ITEM_REGISTRY_ERROR("Drawing item parent must be a drawing.");
		assert(false);
		return false;
	}

	void mvDrawLine::draw(ImDrawList* drawlist, float x, float y)
	{

		mvVec2 start = { x, y };
		drawlist->AddLine(m_p1 + start, m_p2 + start, m_color, m_thickness);

	}

	void mvDrawLine::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;


		if (PyObject* item = PyDict_GetItemString(dict, "p1")) m_p1 = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "p2")) m_p2 = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) m_thickness = ToFloat(item);

	}

	void mvDrawLine::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "p1", ToPyPair(m_p1.x, m_p1.y));
		PyDict_SetItemString(dict, "p2", ToPyPair(m_p2.x, m_p2.y));
		PyDict_SetItemString(dict, "color", ToPyColor(m_color));
		PyDict_SetItemString(dict, "thickness", ToPyFloat(m_thickness));
	}

	PyObject* mvDrawLine::draw_line(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* p1, * p2;
		PyObject* color;
		int thickness;

		static int i = 0; i++;
		std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
		const char* name = sname.c_str();
		const char* before = "";
		const char* parent = "";
		int show = true;

		if (!(mvApp::GetApp()->getParsers())[s_parser].parse(args, kwargs, __FUNCTION__, 
			&p1, &p2, &color, &thickness, &name, &parent, &before, &show))
			return GetPyNone();

		mvVec2 mp1 = ToVec2(p1);
		mvVec2 mp2 = ToVec2(p2);
		mvColor mcolor = ToColor(color);

		auto item = CreateRef<mvDrawLine>(name, mp1, mp2, mcolor, thickness);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before);

		return ToPyString(name);
	}
}