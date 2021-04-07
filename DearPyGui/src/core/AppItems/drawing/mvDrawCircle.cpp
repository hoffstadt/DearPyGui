#include "mvDrawCircle.h"
#include "mvLog.h"
#include "mvItemRegistry.h"

namespace Marvel {

	void mvDrawCircle::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::String, "name", "", "''"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "center"},
			{mvPythonDataType::Float, "radius"},
			{mvPythonDataType::IntList, "color"},	
			{mvPythonDataType::Integer, "segments", "", "0"},
			{mvPythonDataType::Float, "thickness", "", "1.0"},
			{mvPythonDataType::FloatList, "fill", "", "(0, 0, 0, -1)"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Draws a circle on a drawing.", "None", "Drawing") });
	}

	mvDrawCircle::mvDrawCircle(const std::string& name)
		:
		mvAppItem(name)
	{
	}

	bool mvDrawCircle::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvDrawing) return true;
		if (type == mvAppItemType::mvWindowAppItem) return true;

		mvThrowPythonError(1000, "Drawing item parent must be a drawing.");
		MV_ITEM_REGISTRY_ERROR("Drawing item parent must be a drawing.");
		assert(false);
		return false;
	}

	void mvDrawCircle::draw(ImDrawList* drawlist, float x, float y)
	{

		mvVec2 start = { x, y };
		if (m_fill.r > 0.0f)
			drawlist->AddCircleFilled(m_center + start, m_radius, m_fill, m_segments);

		drawlist->AddCircle(m_center + start, m_radius, m_color, m_segments, m_thickness);

	}

	void mvDrawCircle::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;


		if (PyObject* item = PyDict_GetItemString(dict, "center")) m_center = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "fill")) m_fill = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) m_thickness = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "radius")) m_radius = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "segments")) m_segments = ToInt(item);

	}

	void mvDrawCircle::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "center", ToPyPair(m_center.x, m_center.y));
		PyDict_SetItemString(dict, "color", ToPyColor(m_color));
		PyDict_SetItemString(dict, "fill", ToPyColor(m_fill));
		PyDict_SetItemString(dict, "thickness", ToPyFloat(m_thickness));
		PyDict_SetItemString(dict, "radius", ToPyFloat(m_radius));
		PyDict_SetItemString(dict, "segments", ToPyInt(m_segments));
	}

}