#include "mvDrawRect.h"
#include "mvLog.h"
#include "mvItemRegistry.h"

namespace Marvel {

	void mvDrawRect::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name", "", "''"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "pmin", "top left coordinate"},
			{mvPythonDataType::FloatList, "pmax", "bottom right coordinate"},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::FloatList, "fill", "", "(0, 0, 0, -1)"},
			{mvPythonDataType::Float, "rounding", "", "0.0"},
			{mvPythonDataType::Float, "thickness", "", "1.0"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Draws a line on a drawing.", "None", "Drawing") });
	}

	mvDrawRect::mvDrawRect(const std::string& name)
		:
		mvAppItem(name)
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

}