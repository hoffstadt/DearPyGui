#include "mvDrawQuad.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	mvDrawQuad::mvDrawQuad(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

	void mvDrawQuad::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvDrawQuad*>(item);
		_p1 = titem->_p1;
		_p2 = titem->_p2;
		_p3 = titem->_p3;
		_p4 = titem->_p4;
		_color = titem->_color;
		_thickness = titem->_thickness;
		_fill = titem->_fill;
	}

	void mvDrawQuad::draw(ImDrawList* drawlist, float x, float y)
	{

		mvVec4  tp1 =  drawInfo->transform * _p1;
		mvVec4  tp2 =  drawInfo->transform * _p2;
		mvVec4  tp3 =  drawInfo->transform * _p3;
		mvVec4  tp4 =  drawInfo->transform * _p4;

		if (drawInfo->perspectiveDivide)
		{
			tp1.x = tp1.x / tp1.w;
			tp2.x = tp2.x / tp2.w;
			tp3.x = tp3.x / tp3.w;
			tp4.x = tp4.x / tp4.w;

			tp1.y = tp1.y / tp1.w;
			tp2.y = tp2.y / tp2.w;
			tp3.y = tp3.y / tp3.w;
			tp4.y = tp4.y / tp4.w;

			tp1.z = tp1.z / tp1.w;
			tp2.z = tp2.z / tp2.w;
			tp3.z = tp3.z / tp3.w;
			tp4.z = tp4.z / tp4.w;
		}

		if (drawInfo->depthClipping)
		{
			if (mvClipPoint(drawInfo->clipViewport, tp1)) return;
			if (mvClipPoint(drawInfo->clipViewport, tp2)) return;
			if (mvClipPoint(drawInfo->clipViewport, tp3)) return;
			if (mvClipPoint(drawInfo->clipViewport, tp4)) return;
		}

		if (ImPlot::GetCurrentContext()->CurrentPlot)
		{
			drawlist->AddQuad(ImPlot::PlotToPixels(tp1), ImPlot::PlotToPixels(tp2), ImPlot::PlotToPixels(tp3),
				ImPlot::PlotToPixels(tp4), _color, ImPlot::GetCurrentContext()->Mx * _thickness);
			if (_fill.r < 0.0f)
				return;
			drawlist->AddQuadFilled(ImPlot::PlotToPixels(tp1), ImPlot::PlotToPixels(tp2), ImPlot::PlotToPixels(tp3),
				ImPlot::PlotToPixels(tp4), _fill);
		}
		else
		{
			mvVec2 start = { x, y };
			drawlist->AddQuad(tp1 + start, tp2 + start, tp3 + start, tp4 + start, _color, _thickness);
			if (_fill.r < 0.0f)
				return;
			drawlist->AddQuadFilled(tp1 + start, tp2 + start, tp3 + start, tp4 + start, _fill);
		}
	}

	void mvDrawQuad::handleSpecificRequiredArgs(PyObject* dict)
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

			case 2:
				_p3 = ToVec4(item);
				_p3.w = 1.0f;
				break;

			case 3:
				_p4 = ToVec4(item);
				_p4.w = 1.0f;
				break;

			default:
				break;
			}
		}
	}

	void mvDrawQuad::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "p1")) _p1 = ToVec4(item);
		if (PyObject* item = PyDict_GetItemString(dict, "p2")) _p2 = ToVec4(item);
		if (PyObject* item = PyDict_GetItemString(dict, "p3")) _p3 = ToVec4(item);
		if (PyObject* item = PyDict_GetItemString(dict, "p4")) _p4 = ToVec4(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "fill")) _fill = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) _thickness = ToFloat(item);

		_p1.w = 1.0f;
		_p2.w = 1.0f;
		_p3.w = 1.0f;
		_p4.w = 1.0f;


	}

	void mvDrawQuad::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "p1", mvPyObject(ToPyPair(_p1.x, _p1.y)));
		PyDict_SetItemString(dict, "p2", mvPyObject(ToPyPair(_p2.x, _p2.y)));
		PyDict_SetItemString(dict, "p3", mvPyObject(ToPyPair(_p3.x, _p3.y)));
		PyDict_SetItemString(dict, "p4", mvPyObject(ToPyPair(_p4.x, _p4.y)));
		PyDict_SetItemString(dict, "color", mvPyObject(ToPyColor(_color)));
		PyDict_SetItemString(dict, "fill", mvPyObject(ToPyColor(_fill)));
		PyDict_SetItemString(dict, "thickness", mvPyObject(ToPyFloat(_thickness)));
	}

}