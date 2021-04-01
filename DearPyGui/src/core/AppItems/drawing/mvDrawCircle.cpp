#include "mvDrawCircle.h"
#include "mvLog.h"
#include "mvItemRegistry.h"

namespace Marvel {

	void mvDrawCircle::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
			{mvPythonDataType::FloatList, "center"},
			{mvPythonDataType::Float, "radius"},
			{mvPythonDataType::IntList, "color"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Integer, "segments", "", "0"},
			{mvPythonDataType::Float, "thickness", "", "1.0"},
			{mvPythonDataType::FloatList, "fill", "", "(0, 0, 0, -1)"},
			{mvPythonDataType::String, "name", "", "''"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
		}, "Draws a circle on a drawing.", "None", "Drawing") });
	}

	mvDrawCircle::mvDrawCircle(const std::string& name, const mvVec2& center, float radius, const mvColor& color,
		int segments, float thickness, const mvColor& fill)
		:
		mvAppItem(name),
		m_center(center),
		m_radius(radius),
		m_segments(segments),
		m_color(color),
		m_fill(fill),
		m_thickness(thickness)
	{
		m_description.target = 0;
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

	PyObject* mvDrawCircle::draw_circle(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* center;
		float radius;
		PyObject* color;
		int segments = 0;
		float thickness = 1.0f;
		PyObject* fill = nullptr;

		static int i = 0; i++;
		std::string sname = std::string(std::string("$$DPG_") + s_internal_id + std::to_string(i));
		const char* name = sname.c_str();
		const char* before = "";
		const char* parent = "";
		int show = true;

		if (!(mvApp::GetApp()->getParsers())[s_command].parse(args, kwargs, __FUNCTION__, 
			&center, &radius, &color, &segments, &thickness, &fill, &name, &parent, &before, &show))
			return GetPyNone();

		mvVec2 mcenter = ToVec2(center);
		mvColor mcolor = ToColor(color);
		mvColor mfill = ToColor(fill);

		auto item = CreateRef<mvDrawCircle>(name, mcenter, radius, mcolor, segments, thickness, mfill);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before);

		return ToPyString(name);
	}
}