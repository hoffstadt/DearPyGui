#include "mvDrawPolylineCmd.h"
#include "PythonUtilities/mvPythonTranslator.h"

namespace Marvel {

	mvDrawPolylineCmd::mvDrawPolylineCmd(const std::vector<mvVec2>& points, const mvColor& color, bool closed, float thickness)
		:
		m_points(points),
		m_color(color),
		m_closed(closed),
		m_thickness(thickness)
	{
	}

	void mvDrawPolylineCmd::draw(ImDrawList* drawlist, float x, float y)
	{
		mvVec2 start = {x, y};

		std::vector<mvVec2> points = m_points;

		for (auto& point : points)
			point = point + start;

		drawlist->AddPolyline((const ImVec2*)const_cast<const mvVec2*>(points.data()), (int)m_points.size(), m_color, m_closed, m_thickness);
	}

	void mvDrawPolylineCmd::setConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;

		if (PyObject* item = PyDict_GetItemString(dict, "points")) m_points = ToVectVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "closed")) m_closed = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) m_thickness = ToFloat(item);

	}

	void mvDrawPolylineCmd::getConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		PyDict_SetItemString(dict, "points", ToPyList(m_points));
		PyDict_SetItemString(dict, "closed", ToPyBool(m_closed));
		PyDict_SetItemString(dict, "color", ToPyColor(m_color));
		PyDict_SetItemString(dict, "thickness", ToPyFloat(m_thickness));
	}
}