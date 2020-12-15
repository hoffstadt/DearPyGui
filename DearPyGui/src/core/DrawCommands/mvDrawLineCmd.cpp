#include "mvDrawLineCmd.h"
#include "mvPythonTranslator.h"
#include "mvGlobalIntepreterLock.h"
#include "mvApp.h"

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

	PyObject* draw_line(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* drawing;
		int thickness;
		PyObject* p1, * p2;
		PyObject* color;
		const char* tag = "";

		if (!(*mvApp::GetApp()->getParsers())["draw_line"].parse(args, kwargs, __FUNCTION__, &drawing, &p1, &p2, &color, &thickness, &tag))
			return GetPyNone();

		mvVec2 mp1 = ToVec2(p1);
		mvVec2 mp2 = ToVec2(p2);
		mvColor mcolor = ToColor(color);

		mvDrawList* drawlist = GetDrawListFromTarget(drawing);
		if (drawlist)
		{

			if (auto command = drawlist->getCommand(tag))
				*static_cast<mvDrawLineCmd*>(command) = mvDrawLineCmd(mp1, mp2, mcolor, (float)thickness);
			else
			{
				auto cmd = CreateRef<mvDrawLineCmd>(mp1, mp2, mcolor, (float)thickness);
				cmd->tag = tag;
				drawlist->addCommand(cmd);
			}
		}
		return GetPyNone();
	}
}