#include "mvDrawQuadCmd.h"
#include "mvPythonTranslator.h"
#include "mvGlobalIntepreterLock.h"
#include "mvApp.h"

namespace Marvel {

	mvDrawQuadCmd::mvDrawQuadCmd(const mvVec2& p1, const mvVec2& p2, const mvVec2& p3, const mvVec2& p4, const mvColor& color, const mvColor& fill, float thickness)
		:
		m_p1(p1),
		m_p2(p2),
		m_p3(p3),
		m_p4(p4),
		m_color(color),
		m_fill(fill),
		m_thickness(thickness)
	{
	}

	void mvDrawQuadCmd::draw(ImDrawList* drawlist, float x, float y)
	{
		mvVec2 start = {x, y};
		if (m_fill.specified)
			drawlist->AddQuadFilled(m_p1 + start, m_p2 + start, m_p3 + start, m_p4 + start, m_fill);
		drawlist->AddQuad(m_p1 + start, m_p2 + start, m_p3 + start, m_p4 + start, m_color, m_thickness);
	}

	void mvDrawQuadCmd::setConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 

		if (PyObject* item = PyDict_GetItemString(dict, "p1")) m_p1 = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "p2")) m_p2 = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "p3")) m_p3 = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "p4")) m_p4 = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "fill")) m_fill = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) m_thickness = ToFloat(item);

	}

	void mvDrawQuadCmd::getConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "p1", ToPyPair(m_p1.x, m_p1.y));
		PyDict_SetItemString(dict, "p2", ToPyPair(m_p2.x, m_p2.y));
		PyDict_SetItemString(dict, "p3", ToPyPair(m_p3.x, m_p3.y));
		PyDict_SetItemString(dict, "p4", ToPyPair(m_p4.x, m_p4.y));
		PyDict_SetItemString(dict, "color", ToPyColor(m_color));
		PyDict_SetItemString(dict, "fill", ToPyColor(m_fill));
		PyDict_SetItemString(dict, "thickness", ToPyFloat(m_thickness));
	}

	PyObject* draw_quad(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		float thickness = 1.0f;
		PyObject* p1, * p2, * p3, * p4;
		PyObject* color;
		PyObject* fill = nullptr;
		const char* tag = "";

		if (!(*mvApp::GetApp()->getParsers())["draw_quad"].parse(args, kwargs, __FUNCTION__, &drawing, &p1, &p2, &p3, &p4, &color, &fill, &thickness, &tag))
			return GetPyNone();


		mvVec2 mp1 = ToVec2(p1);
		mvVec2 mp2 = ToVec2(p2);
		mvVec2 mp3 = ToVec2(p3);
		mvVec2 mp4 = ToVec2(p4);
		mvColor mcolor = ToColor(color);
		mvColor mfill = ToColor(fill);

		mvDrawList* drawlist = GetDrawListFromTarget(drawing);
		if (drawlist)
		{
			auto cmd = CreateRef<mvDrawQuadCmd>(mp1, mp2, mp3, mp4, mcolor, mfill, thickness);
			cmd->tag = tag;
			drawlist->addCommand(cmd);
		}
		return GetPyNone();
	}
}