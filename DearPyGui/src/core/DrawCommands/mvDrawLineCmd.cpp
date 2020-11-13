#include "mvDrawLineCmd.h"
#include "PythonUtilities/mvPythonTranslator.h"

namespace Marvel {

	mvDrawLineCmd::mvDrawLineCmd(const mvVec2& p1, const mvVec2& p2, const mvColor& color, float thickness)
		:
		m_p1(p1),
		m_p2(p2),
		m_color(color),
		m_thickness(thickness)
	{
	}

	void mvDrawLineCmd::draw(ImDrawList* drawlist, float x, float y)
	{
		mvVec2 start = {x, y};
		drawlist->AddLine(m_p1 + start, m_p2 + start, m_color, m_thickness);
	}

	void mvDrawLineCmd::setConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;

		if (PyObject* item = PyDict_GetItemString(dict, "p1")) m_p1 = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "p2")) m_p2 = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) m_thickness = ToFloat(item);

	}

	void mvDrawLineCmd::getConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		PyDict_SetItemString(dict, "p1", ToPyPair(m_p1.x, m_p1.y));
		PyDict_SetItemString(dict, "p2", ToPyPair(m_p2.x, m_p2.y));
		PyDict_SetItemString(dict, "color", ToPyColor(m_color));
		PyDict_SetItemString(dict, "thickness", ToPyFloat(m_thickness));
	}

}