#include "mvDrawPolyline.h"
#include "mvLog.h"
#include "mvItemRegistry.h"

namespace Marvel {

	void mvDrawPolyline::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name", "", "''"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::ListFloatList, "points"},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::Bool, "closed", "", "False"},
			{mvPythonDataType::Float, "thickness", "", "1.0"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Draws lines on a drawing.", "None", "Drawing") });
	}

	mvDrawPolyline::mvDrawPolyline(const std::string& name)
		:
		mvAppItem(name)
	{
	}

	bool mvDrawPolyline::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvDrawing) return true;
		if (type == mvAppItemType::mvWindowAppItem) return true;

		mvThrowPythonError(1000, "Drawing item parent must be a drawing.");
		MV_ITEM_REGISTRY_ERROR("Drawing item parent must be a drawing.");
		assert(false);
		return false;
	}

	void mvDrawPolyline::draw(ImDrawList* drawlist, float x, float y)
	{
		mvVec2 start = { x, y };

		std::vector<mvVec2> points = m_points;

		for (auto& point : points)
			point = point + start;

		drawlist->AddPolyline((const ImVec2*)const_cast<const mvVec2*>(points.data()), (int)m_points.size(), m_color, m_closed, m_thickness);
	}

	void mvDrawPolyline::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "points")) m_points = ToVectVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "closed")) m_closed = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) m_thickness = ToFloat(item);
	}

	void mvDrawPolyline::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "points", ToPyList(m_points));
		PyDict_SetItemString(dict, "closed", ToPyBool(m_closed));
		PyDict_SetItemString(dict, "color", ToPyColor(m_color));
		PyDict_SetItemString(dict, "thickness", ToPyFloat(m_thickness));
	}
}