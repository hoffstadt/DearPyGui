#include "mvDrawRect.h"
#include "mvLog.h"
#include "mvItemRegistry.h"

namespace Marvel {

	void mvDrawRect::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::FloatList, "pmin", "top left coordinate"},
			{mvPythonDataType::FloatList, "pmax", "bottom right coordinate"},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "fill", "", "(0, 0, 0, -1)"},
			{mvPythonDataType::Float, "rounding", "", "0.0"},
			{mvPythonDataType::Float, "thickness", "", "1.0"},
			{mvPythonDataType::String, "name", "", "''"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Draws a line on a drawing.", "None", "Drawing") });
	}

	mvDrawRect::mvDrawRect(const std::string& name, const mvVec2& pmin, const mvVec2& pmax, const mvColor& color,
		const mvColor& fill, float rounding, float thickness)
		:
		mvAppItem(name),
		m_pmin(pmin),
		m_pmax(pmax),
		m_rounding(rounding),
		m_color(color),
		m_fill(fill),
		m_thickness(thickness)
	{
	}

	bool mvDrawRect::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvDrawing) return true;
		if (type == mvAppItemType::mvWindowAppItem) return true;

		mvThrowPythonError(1000, "Drawing item parent must be a drawing.");
		MV_ITEM_REGISTRY_ERROR("Drawing item parent must be a drawing.");
		assert(false);
		return false;
	}

	void mvDrawRect::draw(ImDrawList* drawlist, float x, float y)
	{
		mvVec2 start = { x, y };
		if (m_fill.r > 0.0f)
			drawlist->AddRectFilled(m_pmin + start, m_pmax + start, m_fill, m_rounding, ImDrawCornerFlags_All);
		drawlist->AddRect(m_pmin + start, m_pmax + start, m_color, m_rounding, ImDrawCornerFlags_All, m_thickness);
	}

	void mvDrawRect::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "pmax")) m_pmax = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "pmin")) m_pmin = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "fill")) m_fill = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "rounding")) m_rounding = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) m_thickness = ToFloat(item);
	}

	void mvDrawRect::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "pmax", ToPyPair(m_pmax.x, m_pmax.y));
		PyDict_SetItemString(dict, "pmin", ToPyPair(m_pmin.x, m_pmin.y));
		PyDict_SetItemString(dict, "color", ToPyColor(m_color));
		PyDict_SetItemString(dict, "fill", ToPyColor(m_fill));
		PyDict_SetItemString(dict, "rounding", ToPyFloat(m_rounding));
		PyDict_SetItemString(dict, "thickness", ToPyFloat(m_thickness));
	}

	PyObject* mvDrawRect::draw_rectangle(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		float thickness = 1.0f, rounding = 0.0f;
		PyObject* pmin, * pmax;
		PyObject* color;
		PyObject* fill = nullptr;

		static int i = 0; i++;
		std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
		const char* name = sname.c_str();
		const char* before = "";
		const char* parent = "";
		int show = true;

		if (!(mvApp::GetApp()->getParsers())[s_command].parse(args, kwargs, __FUNCTION__, 
			&pmin, &pmax, &color, &fill, &rounding, &thickness, &name, &parent, &before, &show))
			return GetPyNone();

		mvVec2 mpmax = ToVec2(pmax);
		mvVec2 mpmin = ToVec2(pmin);
		mvColor mcolor = ToColor(color);
		mvColor mfill = ToColor(fill);

		auto item = CreateRef<mvDrawRect>(name, mpmin, mpmax, mcolor, mfill, rounding, thickness);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before);

		return ToPyString(name);
	}
}