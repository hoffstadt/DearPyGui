#include "mvDrawLine.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	mvDrawLine::mvDrawLine(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

	void mvDrawLine::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvDrawLine*>(item);
		_p1 = titem->_p1;
		_p2 = titem->_p2;
		_color = titem->_color;
		_thickness = titem->_thickness;
	}

	void mvDrawLine::draw(ImDrawList* drawlist, float x, float y)
	{
		mvVec4  tp1 = drawInfo->transform * _p1;
		mvVec4  tp2 = drawInfo->transform * _p2;

		if (drawInfo->perspectiveDivide)
		{
			tp1.x = tp1.x / tp1.w;
			tp2.x = tp2.x / tp2.w;

			tp1.y = tp1.y / tp1.w;
			tp2.y = tp2.y / tp2.w;

			tp1.z = tp1.z / tp1.w;
			tp2.z = tp2.z / tp2.w;
		}

		if (drawInfo->depthClipping)
		{
			if (mvClipPoint(drawInfo->clipViewport, tp1)) return;
			if (mvClipPoint(drawInfo->clipViewport, tp2)) return;
		}

		if(ImPlot::GetCurrentContext()->CurrentPlot)
			drawlist->AddLine(ImPlot::PlotToPixels(tp1), ImPlot::PlotToPixels(tp2), _color,
				ImPlot::GetCurrentContext()->Mx * _thickness);
		else
		{
			mvVec2 start = { x, y };
			drawlist->AddLine(tp1 + start, tp2 + start, _color, _thickness);
		}

	}

	void mvDrawLine::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				_p1 = ToVec4(item);
				_p1.w = 1.0f;
				break;

			case 1:
				_p2 = ToVec4(item);
				_p2.w = 1.0f;
				break;

			default:
				break;
			}
		}
	}

	void mvDrawLine::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;


		if (PyObject* item = PyDict_GetItemString(dict, "p1")) _p1 = ToVec4(item);
		if (PyObject* item = PyDict_GetItemString(dict, "p2")) _p2 = ToVec4(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) _thickness = ToFloat(item);

		_p1.w = 1.0f;
		_p2.w = 1.0f;
	}

	void mvDrawLine::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "p1", mvPyObject(ToPyPair(_p1.x, _p1.y)));
		PyDict_SetItemString(dict, "p2", mvPyObject(ToPyPair(_p2.x, _p2.y)));
		PyDict_SetItemString(dict, "color", mvPyObject(ToPyColor(_color)));
		PyDict_SetItemString(dict, "thickness", mvPyObject(ToPyFloat(_thickness)));
	}

}