#include "mvDrawCircleCmd.h"
#include "mvPythonTranslator.h"
#include "mvGlobalIntepreterLock.h"
#include "mvApp.h"

namespace Marvel {

	mvDrawCircleCmd::mvDrawCircleCmd(const mvVec2& center, float radius, const mvColor& color,
		int segments, float thickness, const mvColor& fill)
		:
		m_center(center),
		m_radius(radius),
		m_segments(segments),
		m_color(color),
		m_fill(fill),
		m_thickness(thickness)
	{
	}

	void mvDrawCircleCmd::draw(ImDrawList* drawlist, float x, float y)
	{
		mvVec2 start = {x, y};
		if (m_fill.specified)
			drawlist->AddCircleFilled(m_center + start, m_radius, m_fill, m_segments);

		drawlist->AddCircle(m_center + start, m_radius, m_color, m_segments, m_thickness);
	}

	void mvDrawCircleCmd::setConfigDict(PyObject* dict)
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

	void mvDrawCircleCmd::getConfigDict(PyObject* dict)
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

	PyObject* draw_circle(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		PyObject* center;
		float radius;
		PyObject* color;
		int segments = 0;
		float thickness = 1.0f;
		PyObject* fill = nullptr;
		const char* tag = "";

		if (!(*mvApp::GetApp()->getParsers())["draw_circle"].parse(args, kwargs, __FUNCTION__, &drawing, &center, &radius, &color, &segments, &thickness, &fill, &tag))
			return GetPyNone();

		mvVec2 mcenter = ToVec2(center);
		mvColor mcolor = ToColor(color);
		mvColor mfill = ToColor(fill);

		auto cmd = CreateRef<mvDrawCircleCmd>(mcenter, radius, mcolor, segments, thickness, mfill);
		cmd->tag = tag;

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		mvRef<mvDrawList> drawlist = GetDrawListFromTarget(drawing);
		if (drawlist)
			drawlist->addCommand(cmd);

		return GetPyNone();
	}
}