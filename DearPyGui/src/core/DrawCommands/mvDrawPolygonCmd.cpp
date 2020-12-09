#include "mvDrawPolygonCmd.h"
#include "mvPythonTranslator.h"
#include "mvGlobalIntepreterLock.h"

namespace Marvel {

	mvDrawPolygonCmd::mvDrawPolygonCmd(const std::vector<mvVec2>& points, const mvColor& color, const mvColor& fill, float thickness)
		:
		m_points(points),
		m_color(color),
		m_fill(fill),
		m_thickness(thickness)
	{
	}

	void mvDrawPolygonCmd::draw(ImDrawList* drawlist, float x, float y)
	{
		mvVec2 start = {x, y};
		std::vector<mvVec2> points = m_points;
		for (auto& point : points)
			point = point + start;
		drawlist->AddPolyline((const ImVec2*)const_cast<const mvVec2*>(points.data()), (int)m_points.size(), m_color, false, m_thickness);

		if (m_fill.specified)
		{
			size_t i;
			int y;
			int miny, maxy;
			int x1, y1;
			int x2, y2;
			int ind1, ind2;
			size_t ints;
			size_t n = m_points.size();
			int* polyints = new int[n];

			/* Determine Y maxima */
			miny = (int)m_points[0].y;
			maxy = (int)m_points[0].y;
			for (i = 1; i < n; i++)
			{
				miny = std::min(miny, (int)m_points[i].y);
				maxy = std::max(maxy, (int)m_points[i].y);
			}

			/* Draw, scanning y */
			for (y = miny; y <= maxy; y++) {
				ints = 0;
				for (i = 0; (i < n); i++) {
					if (!i)
					{
						ind1 = (int)n - 1;
						ind2 = 0;
					}
					else
					{
						ind1 = (int)i - 1;
						ind2 = (int)i;
					}
					y1 = (int)m_points[ind1].y;
					y2 = (int)m_points[ind2].y;
					if (y1 < y2)
					{
						x1 = (int)m_points[ind1].x;
						x2 = (int)m_points[ind2].x;
					}
					else if (y1 > y2)
					{
						y2 = (int)m_points[ind1].y;
						y1 = (int)m_points[ind2].y;
						x2 = (int)m_points[ind1].x;
						x1 = (int)m_points[ind2].x;
					}
					else
						continue;

					if (((y >= y1) && (y < y2)) || ((y == maxy) && (y > y1) && (y <= y2)))
						polyints[ints++] = (y - y1) * (x2 - x1) / (y2 - y1) + x1;

				}

				auto compare_int = [](const void* a, const void* b)
				{
					return (*(const int*)a) - (*(const int*)b);
				};

				qsort(polyints, ints, sizeof(int), compare_int);

				for (i = 0; i < ints; i += 2)
				{
					drawlist->AddLine({ (float)polyints[i] + start.x, (float)y + start.y },
						{ (float)polyints[i + 1] + start.x, (float)y + start.y }, m_fill, m_thickness);
				}
			}
			delete[] polyints;
		}
	}

	void mvDrawPolygonCmd::setConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;

		if (PyObject* item = PyDict_GetItemString(dict, "points")) m_points = ToVectVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "fill")) m_fill = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) m_thickness = ToFloat(item);

	}

	void mvDrawPolygonCmd::getConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		PyDict_SetItemString(dict, "points", ToPyList(m_points));
		PyDict_SetItemString(dict, "fill", ToPyColor(m_fill));
		PyDict_SetItemString(dict, "color", ToPyColor(m_color));
		PyDict_SetItemString(dict, "thickness", ToPyFloat(m_thickness));
	}
}