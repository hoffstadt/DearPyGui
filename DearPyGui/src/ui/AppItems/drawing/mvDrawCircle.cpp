#include "mvDrawCircle.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	mvDrawCircle::mvDrawCircle(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

	void mvDrawCircle::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvDrawCircle*>(item);
		_center = titem->_center;
		_radius = titem->_radius;
		_segments = titem->_segments;
		_color = titem->_color;
		_fill = titem->_fill;
		_thickness = titem->_thickness;
	}

	void mvDrawCircle::draw(ImDrawList* drawlist, float x, float y)
	{
		mvVec4  tcenter = drawInfo->transform * _center;

		if (drawInfo->perspectiveDivide)
		{
			tcenter.x = tcenter.x / tcenter.w;
			tcenter.y = tcenter.y / tcenter.w;
			tcenter.z = tcenter.z / tcenter.w;
		}

		if (drawInfo->depthClipping)
		{
			if (mvClipPoint(drawInfo->clipViewport, tcenter)) return;
		}

		if (ImPlot::GetCurrentContext()->CurrentPlot)
		{
			drawlist->AddCircle(ImPlot::PlotToPixels(tcenter), ImPlot::GetCurrentContext()->Mx * _radius, _color,
				ImPlot::GetCurrentContext()->Mx * _segments, _thickness);
			if (_fill.r < 0.0f)
				return;
			drawlist->AddCircleFilled(ImPlot::PlotToPixels(tcenter), ImPlot::GetCurrentContext()->Mx * _radius, _fill, _segments);
		}
		else
		{
			mvVec2 start = { x, y };
			drawlist->AddCircle(tcenter + start, _radius, _color, _segments, _thickness);
			if (_fill.r < 0.0f)
				return;
			drawlist->AddCircleFilled(tcenter + start, _radius, _fill, _segments);
		}
	}

	void mvDrawCircle::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				_center = ToVec4(item);
				_center.w = 1.0f;
				break;

			case 1:
				_radius = ToFloat(item);
				break;

			default:
				break;
			}
		}
	}

	void mvDrawCircle::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;


		if (PyObject* item = PyDict_GetItemString(dict, "center")) _center = ToVec4(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "fill")) _fill = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) _thickness = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "radius")) _radius = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "segments")) _segments = ToInt(item);

		_center.w = 1.0f;
	}

	void mvDrawCircle::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "center", mvPyObject(ToPyPair(_center.x, _center.y)));
		PyDict_SetItemString(dict, "color", mvPyObject(ToPyColor(_color)));
		PyDict_SetItemString(dict, "fill", mvPyObject(ToPyColor(_fill)));
		PyDict_SetItemString(dict, "thickness", mvPyObject(ToPyFloat(_thickness)));
		PyDict_SetItemString(dict, "radius", mvPyObject(ToPyFloat(_radius)));
		PyDict_SetItemString(dict, "segments", mvPyObject(ToPyInt(_segments)));
	}

}