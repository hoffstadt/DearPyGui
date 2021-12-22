#include "mvDrawRect.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvPyObject.h"

namespace Marvel {

	mvDrawRect::mvDrawRect(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

	void mvDrawRect::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvDrawRect*>(item);
		_pmin = titem->_pmin;
		_pmax = titem->_pmax;
		_rounding = titem->_rounding;
		_color = titem->_color;
		_color_upper_left = titem->_color_upper_left;
		_color_upper_right = titem->_color_upper_right;
		_color_bottom_left = titem->_color_bottom_left;
		_color_bottom_right = titem->_color_bottom_right;
		_fill = titem->_fill;
		_thickness = titem->_thickness;
		_multicolor = titem->_multicolor;
	}

	void mvDrawRect::draw(ImDrawList* drawlist, float x, float y)
	{
		mvVec4  tpmin = drawInfo->transform * _pmin;
		mvVec4  tpmax = drawInfo->transform * _pmax;

		if (drawInfo->perspectiveDivide)
		{
			tpmin.x = tpmin.x / tpmin.w;
			tpmax.x = tpmax.x / tpmax.w;

			tpmin.y = tpmin.y / tpmin.w;
			tpmax.y = tpmax.y / tpmax.w;

			tpmin.z = tpmin.z / tpmin.w;
			tpmax.z = tpmax.z / tpmax.w;
		}

		if (drawInfo->depthClipping)
		{
			if (mvClipPoint(drawInfo->clipViewport, tpmin)) return;
			if (mvClipPoint(drawInfo->clipViewport, tpmax)) return;
		}

		if (ImPlot::GetCurrentContext()->CurrentPlot)
		{
			drawlist->AddRect(ImPlot::PlotToPixels(tpmin), ImPlot::PlotToPixels(tpmax), _color,
				ImPlot::GetCurrentContext()->Mx * _rounding, ImDrawCornerFlags_All, ImPlot::GetCurrentContext()->Mx * _thickness);
			if (_multicolor)
			{
				drawlist->AddRectFilledMultiColor(ImPlot::PlotToPixels(tpmin), ImPlot::PlotToPixels(tpmax), _color_bottom_right, _color_bottom_left, _color_upper_left, _color_upper_right);
				return;
			}
			if (_fill.r < 0.0f)
				return;
			drawlist->AddRectFilled(ImPlot::PlotToPixels(tpmin), ImPlot::PlotToPixels(tpmax), _fill, ImPlot::GetCurrentContext()->Mx * _rounding, ImDrawCornerFlags_All);
		}
		else
		{
			mvVec2 start = { x, y };
			drawlist->AddRect(tpmin + start, tpmax + start, _color, _rounding, ImDrawCornerFlags_All, _thickness);
			if (_multicolor)
			{
				drawlist->AddRectFilledMultiColor(tpmin + start, tpmax + start, _color_bottom_right, _color_bottom_left, _color_upper_left, _color_upper_right);
				return;
			}
			if (_fill.r < 0.0f)
				return;
			drawlist->AddRectFilled(tpmin + start, tpmax + start, _fill, _rounding, ImDrawCornerFlags_All);
		}
	}

	void mvDrawRect::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				_pmin = ToVec4(item);
				_pmin.w = 1.0f;
				break;

			case 1:
				_pmax = ToVec4(item);
				_pmax.w = 1.0f;
				break;

			default:
				break;
			}
		}
	}

	void mvDrawRect::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "pmax")) _pmax = ToVec4(item);
		if (PyObject* item = PyDict_GetItemString(dict, "pmin")) _pmin = ToVec4(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color_upper_left")) _color_upper_left = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color_upper_right")) _color_upper_right = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color_bottom_left")) _color_bottom_left = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color_bottom_right")) _color_bottom_right = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "fill")) _fill = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "rounding")) _rounding = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) _thickness = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "multicolor")) _multicolor = ToBool(item);

		if (_multicolor)
			_rounding = 0.0f;
	}

	void mvDrawRect::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "pmax", mvPyObject(ToPyPair(_pmax.x, _pmax.y)));
		PyDict_SetItemString(dict, "pmin", mvPyObject(ToPyPair(_pmin.x, _pmin.y)));
		PyDict_SetItemString(dict, "color", mvPyObject(ToPyColor(_color)));
		PyDict_SetItemString(dict, "fill", mvPyObject(ToPyColor(_fill)));
		PyDict_SetItemString(dict, "rounding", mvPyObject(ToPyFloat(_rounding)));
		PyDict_SetItemString(dict, "thickness", mvPyObject(ToPyFloat(_thickness)));
	}

}