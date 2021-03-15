#include "mvDrawPolylineCmd.h"

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
		 

		if (PyObject* item = PyDict_GetItemString(dict, "points")) m_points = ToVectVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "closed")) m_closed = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) m_thickness = ToFloat(item);

	}

	void mvDrawPolylineCmd::getConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "points", ToPyList(m_points));
		PyDict_SetItemString(dict, "closed", ToPyBool(m_closed));
		PyDict_SetItemString(dict, "color", ToPyColor(m_color));
		PyDict_SetItemString(dict, "thickness", ToPyFloat(m_thickness));
	}

	PyObject* draw_polyline(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		PyObject* points;
		PyObject* color;
		int closed = false;
		float thickness = 1.0f;
		const char* tag = "";

		if (!(mvApp::GetApp()->getParsers())["draw_polyline"].parse(args, kwargs, __FUNCTION__, &drawing, &points, &color, &closed, &thickness, &tag))
			return GetPyNone();

		auto mpoints = ToVectVec2(points);
		mvColor mcolor = ToColor(color);

		auto cmd = CreateRef<mvDrawPolylineCmd>(mpoints, mcolor, closed, thickness);
		cmd->tag = tag;

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		mvRef<mvDrawList> drawlist = GetDrawListFromTarget(drawing);
		if (drawlist)
			drawlist->addCommand(cmd);

		return GetPyNone();
	}
}