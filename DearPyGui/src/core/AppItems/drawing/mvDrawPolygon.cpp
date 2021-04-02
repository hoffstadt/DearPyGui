#include "mvDrawPolygon.h"
#include "mvLog.h"
#include "mvItemRegistry.h"

namespace Marvel {

	void mvDrawPolygon::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::ListFloatList, "points"},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::FloatList, "fill", "", "(0, 0, 0, -1)"},
			{mvPythonDataType::Float, "thickness", "", "1.0"},
			{mvPythonDataType::String, "name", "", "''"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Draws a polygon on a drawing.", "None", "Drawing") });
	}

	mvDrawPolygon::mvDrawPolygon(const std::string& name, const std::vector<mvVec2>& points,
		const mvColor& color, const mvColor& fill, float thickness)
		:
		mvAppItem(name),
		m_points(points),
		m_color(color),
		m_fill(fill),
		m_thickness(thickness)
	{
	}

	bool mvDrawPolygon::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvDrawing) return true;
		if (type == mvAppItemType::mvWindowAppItem) return true;

		mvThrowPythonError(1000, "Drawing item parent must be a drawing.");
		MV_ITEM_REGISTRY_ERROR("Drawing item parent must be a drawing.");
		assert(false);
		return false;
	}

	void mvDrawPolygon::draw(ImDrawList* drawlist, float x, float y)
	{
		mvVec2 start = { x, y };
		std::vector<mvVec2> points = m_points;
		for (auto& point : points)
			point = point + start;

		if (m_fill.r > 0.0f)
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

		drawlist->AddPolyline((const ImVec2*)const_cast<const mvVec2*>(points.data()), (int)m_points.size(), m_color, false, m_thickness);
	}

	void mvDrawPolygon::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "points")) m_points = ToVectVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "fill")) m_fill = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) m_thickness = ToFloat(item);

	}

	void mvDrawPolygon::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "points", ToPyList(m_points));
		PyDict_SetItemString(dict, "fill", ToPyColor(m_fill));
		PyDict_SetItemString(dict, "color", ToPyColor(m_color));
		PyDict_SetItemString(dict, "thickness", ToPyFloat(m_thickness));

	}

	PyObject* mvDrawPolygon::draw_polygon(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* points;
		PyObject* color;
		PyObject* fill = nullptr;
		float thickness = 1.0f;

		static int i = 0; i++;
		std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
		const char* name = sname.c_str();
		const char* before = "";
		const char* parent = "";
		int show = true;

		if (!(mvApp::GetApp()->getParsers())[s_command].parse(args, kwargs, __FUNCTION__, 
			&points, &color, &fill, &thickness, &name, &parent, &before, &show))
			return GetPyNone();

		auto mpoints = ToVectVec2(points);
		mvColor mcolor = ToColor(color);
		mvColor mfill = ToColor(fill);

		auto item = CreateRef<mvDrawPolygon>(name, mpoints, mcolor, mfill, thickness);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before);

		return ToPyString(name);
	}
}