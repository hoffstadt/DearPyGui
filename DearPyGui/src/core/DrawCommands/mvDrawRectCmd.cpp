#include"mvDrawRectCmd.h"
#include "PythonUtilities/mvPythonTranslator.h"

namespace Marvel {

	mvDrawRectCmd::mvDrawRectCmd(const mvVec2& pmin, const mvVec2& pmax, const mvColor& color, const mvColor& fill, float rounding, float thickness)
		:
		m_pmin(pmin),
		m_pmax(pmax),
		m_rounding(rounding),
		m_color(color),
		m_fill(fill),
		m_thickness(thickness)
	{
	}

	void mvDrawRectCmd::draw(ImDrawList* drawlist, float x, float y)
	{
		mvVec2 start = {x, y};
		if (m_fill.specified)
			drawlist->AddRectFilled(m_pmin + start, m_pmax + start, m_fill, m_rounding, ImDrawCornerFlags_All);
		drawlist->AddRect(m_pmin + start, m_pmax + start, m_color, m_rounding, ImDrawCornerFlags_All, m_thickness);
	}

	void mvDrawRectCmd::setConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;

		if (PyObject* item = PyDict_GetItemString(dict, "pmax")) m_pmax = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "pmin")) m_pmin = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "fill")) m_fill = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "rounding")) m_rounding = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) m_thickness = ToFloat(item);

	}

	void mvDrawRectCmd::getConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		PyDict_SetItemString(dict, "pmax", ToPyPair(m_pmax.x, m_pmax.y));
		PyDict_SetItemString(dict, "pmin", ToPyPair(m_pmin.x, m_pmin.y));
		PyDict_SetItemString(dict, "color", ToPyColor(m_color));
		PyDict_SetItemString(dict, "fill", ToPyColor(m_fill));
		PyDict_SetItemString(dict, "rounding", ToPyFloat(m_rounding));
		PyDict_SetItemString(dict, "thickness", ToPyFloat(m_thickness));
	}

}