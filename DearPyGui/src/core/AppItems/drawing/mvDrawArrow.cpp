#include "mvDrawArrow.h"
#include "mvLog.h"
#include "mvItemRegistry.h"

namespace Marvel {

	void mvDrawArrow::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::FloatList, "p1"},
			{mvPythonDataType::FloatList, "p2"},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::Integer, "thickness"},
			{mvPythonDataType::Integer, "size"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "name", "", "''"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Draws an arrow on a drawing.", "None", "Drawing") });

	}

	mvDrawArrow::mvDrawArrow(const std::string& name, const mvVec2& p1, const mvVec2& p2,
		const mvColor& color, float thickness, float size)
		:
		mvAppItem(name),
		m_p1(p1),
		m_p2(p2),
		m_color(color),
		m_thickness(thickness),
		m_size(size)
	{
		m_description.target = 0;
		updatePoints();
	}

	void mvDrawArrow::updatePoints()
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

	bool mvDrawArrow::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvDrawing) return true;
		if (type == mvAppItemType::mvWindowAppItem) return true;

		mvThrowPythonError(1000, "Drawing item parent must be a drawing.");
		MV_ITEM_REGISTRY_ERROR("Drawing item parent must be a drawing.");
		assert(false);
		return false;
	}

	void mvDrawArrow::draw(ImDrawList* drawlist, float x, float y)
	{
		mvVec2 start = { x, y };
		drawlist->AddLine(m_p1 + start, m_p2 + start, m_color, m_thickness);
		drawlist->AddTriangle(m_points[0] + start, m_points[1] + start, m_points[2] + start, m_color, m_thickness);
		drawlist->AddTriangleFilled(m_points[0] + start, m_points[1] + start, m_points[2] + start, m_color);
	}

	void mvDrawArrow::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;


		if (PyObject* item = PyDict_GetItemString(dict, "p1")) m_p1 = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "p2")) m_p2 = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) m_thickness = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "size")) m_size = ToFloat(item);

		updatePoints();

	}

	void mvDrawArrow::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		PyDict_SetItemString(dict, "p1", ToPyPair(m_p1.x, m_p1.y));
		PyDict_SetItemString(dict, "p2", ToPyPair(m_p2.x, m_p2.y));
		PyDict_SetItemString(dict, "color", ToPyColor(m_color));
		PyDict_SetItemString(dict, "thickness", ToPyFloat(m_thickness));
		PyDict_SetItemString(dict, "size", ToPyFloat(m_size));
	}

	PyObject* mvDrawArrow::draw_arrow(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* p1, * p2;
		PyObject* color;
		int thickness;
		int size;

		static int i = 0; i++;
		std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
		const char* name = sname.c_str();
		const char* before = "";
		const char* parent = "";
		int show = true;

		if (!(mvApp::GetApp()->getParsers())[s_command].parse(args, kwargs, __FUNCTION__,
			&p1, &p2, &color, &thickness, &size, &name, &parent, &before, &show))
			return GetPyNone();

		mvVec2 mp1 = ToVec2(p1);
		mvVec2 mp2 = ToVec2(p2);
		mvColor mcolor = ToColor(color);

		auto item = CreateRef<mvDrawArrow>(name, mp1, mp2, mcolor, thickness, size);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before);

		return ToPyString(name);
	}
}