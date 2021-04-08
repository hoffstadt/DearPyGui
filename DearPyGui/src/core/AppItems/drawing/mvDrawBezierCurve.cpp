#include "mvDrawBezierCurve.h"
#include "mvLog.h"
#include "mvItemRegistry.h"

namespace Marvel {

	void mvDrawBezierCurve::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("source");
		parser.removeArg("width");
		parser.removeArg("height");
		parser.removeArg("label");
		parser.removeArg("callback");
		parser.removeArg("callback_data");
		parser.removeArg("enabled");

		parser.addArg<mvPyDataType::FloatList>("p1");
		parser.addArg<mvPyDataType::FloatList>("p2");
		parser.addArg<mvPyDataType::FloatList>("p3");
		parser.addArg<mvPyDataType::FloatList>("p4");

		parser.addArg<mvPyDataType::IntList>("color", mvArgType::KEYWORD, "(255, 255, 255, 255)");

		parser.addArg<mvPyDataType::Float>("thickness", mvArgType::KEYWORD, "1.0");

		parser.addArg<mvPyDataType::Integer>("size", mvArgType::KEYWORD, "4");
		parser.addArg<mvPyDataType::Integer>("segments", mvArgType::KEYWORD, "0");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvDrawBezierCurve::mvDrawBezierCurve(const std::string& name)
		:
		mvAppItem(name)
	{
	}

	bool mvDrawBezierCurve::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvDrawing) return true;
		if (type == mvAppItemType::mvWindowAppItem) return true;

		mvThrowPythonError(1000, "Drawing item parent must be a drawing.");
		MV_ITEM_REGISTRY_ERROR("Drawing item parent must be a drawing.");
		assert(false);
		return false;
	}

	void mvDrawBezierCurve::draw(ImDrawList* drawlist, float x, float y)
	{
		mvVec2 start = { x, y };
		drawlist->AddBezierCubic(m_p1 + start, m_p2 + start, m_p3 + start, m_p4 + start, m_color, m_thickness);
	}

	void mvDrawBezierCurve::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "p1")) m_p1 = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "p2")) m_p2 = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "p3")) m_p3 = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "p4")) m_p4 = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) m_thickness = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "segments")) m_segments = ToInt(item);
	}

	void mvDrawBezierCurve::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "p1", ToPyPair(m_p1.x, m_p1.y));
		PyDict_SetItemString(dict, "p2", ToPyPair(m_p2.x, m_p2.y));
		PyDict_SetItemString(dict, "p3", ToPyPair(m_p3.x, m_p3.y));
		PyDict_SetItemString(dict, "p4", ToPyPair(m_p4.x, m_p4.y));
		PyDict_SetItemString(dict, "color", ToPyColor(m_color));
		PyDict_SetItemString(dict, "thickness", ToPyFloat(m_thickness));
		PyDict_SetItemString(dict, "segments", ToPyInt(m_segments));
	}

}