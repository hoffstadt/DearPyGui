#include "mvDrawArrowCmd.h"
#include <cmath>
#include <algorithm>
#undef min
#undef max
#include "PythonUtilities/mvPythonTranslator.h"

namespace Marvel {

	mvDrawArrowCmd::mvDrawArrowCmd(const mvVec2& p1, const mvVec2& p2,
		const mvColor& color, float thickness, float size)
		:
		m_p1(p1),
		m_p2(p2),
		m_color(color),
		m_thickness(thickness),
		m_size(size)
	{
		updatePoints();
	}

	void mvDrawArrowCmd::updatePoints()
	{
		float xsi = m_p1.x;
		float xfi = m_p2.x;
		float ysi = m_p1.y;
		float yfi = m_p2.y;

		// length of arrow head
		double xoffset = m_size;
		double yoffset = m_size;

		// get pointer angle w.r.t +X (in radians)
		double angle = 0.0;
		if (xsi >= xfi && ysi >= yfi) {
			angle = atan((ysi - yfi) / (xsi - xfi));
		}
		else if (xsi < xfi && ysi >= yfi) {
			angle = M_PI + atan((ysi - yfi) / (xsi - xfi));
		}
		else if (xsi < xfi && ysi < yfi) {
			angle = -M_PI + atan((ysi - yfi) / (xsi - xfi));
		}
		else if (xsi >= xfi && ysi < yfi) {
			angle = atan((ysi - yfi) / (xsi - xfi));
		}

		// arrow head points
		auto x1 = (float)(xsi - xoffset * cos(angle));
		auto y1 = (float)(ysi - yoffset * sin(angle));

		m_points.clear();
		m_points.push_back({ xsi, ysi });
		m_points.push_back({ (float)(x1 - 0.5 * m_size * sin(angle)), (float)(y1 + 0.5 * m_size * cos(angle)) });
		m_points.push_back({ (float)(x1 + 0.5 * m_size * cos((M_PI / 2.0) - angle)), (float)(y1 - 0.5 * m_size * sin((M_PI / 2.0) - angle)) });

	}

	void mvDrawArrowCmd::draw(ImDrawList* drawlist, float x, float y)
	{
		mvVec2 start = {x, y};
		drawlist->AddLine(m_p1 + start, m_p2 + start, m_color, m_thickness);
		drawlist->AddTriangle(m_points[0] + start, m_points[1] + start, m_points[2] + start, m_color, m_thickness);
		drawlist->AddTriangleFilled(m_points[0] + start, m_points[1] + start, m_points[2] + start, m_color);
	}

	void mvDrawArrowCmd::setConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;

		if (PyObject* item = PyDict_GetItemString(dict, "p1")) m_p1 = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "p2")) m_p2 = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) m_thickness = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "size")) m_size = ToFloat(item);

		updatePoints();
	}

	void mvDrawArrowCmd::getConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		PyDict_SetItemString(dict, "p1", ToPyPair(m_p1.x, m_p1.y));
		PyDict_SetItemString(dict, "p2", ToPyPair(m_p2.x, m_p2.y));
		PyDict_SetItemString(dict, "color", ToPyColor(m_color));
		PyDict_SetItemString(dict, "thickness", ToPyFloat(m_thickness));
		PyDict_SetItemString(dict, "size", ToPyFloat(m_size));
	}
}