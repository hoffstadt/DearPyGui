#include "mvDrawPolyline.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	mvDrawPolyline::mvDrawPolyline(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

	void mvDrawPolyline::applySpecificTemplate(mvAppItem* item)
	{
		auto titem = static_cast<mvDrawPolyline*>(item);
		_color = titem->_color;
		_closed = titem->_closed;
		_thickness = titem->_thickness;
		_points = titem->_points;
	}

	void mvDrawPolyline::draw(ImDrawList* drawlist, float x, float y)
	{

		mvVec4 start = { x, y };

		std::vector<mvVec4> points = _points;
		std::vector<ImVec2> finalpoints;
		finalpoints.reserve(_points.size());

		for (auto& point : points)
			point = drawInfo->transform * point;

		if (drawInfo->perspectiveDivide)
		{

			for (auto& point : points)
			{
				point.x = point.x / point.w;
				point.x = point.x / point.w;
				point.x = point.x / point.w;
			}
		}

		if (drawInfo->depthClipping)
		{
			for(auto& point : points)
				if (mvClipPoint(drawInfo->clipViewport, point)) return;
		}

		if (ImPlot::GetCurrentContext()->CurrentPlot)
		{
			for (auto& point : points)
			{
				ImVec2 impoint = ImPlot::PlotToPixels(point);
				point.x = impoint.x;
				point.y = impoint.y;
				finalpoints.push_back(impoint);
			}

			drawlist->AddPolyline(finalpoints.data(), (int)finalpoints.size(), _color,
				_closed, ImPlot::GetCurrentContext()->Mx * _thickness);
		}
		else
		{
			for (auto& point : points)
			{
				point = point + start;
				finalpoints.push_back(ImVec2{ point.x, point.y });
			}

			drawlist->AddPolyline(finalpoints.data(), (int)finalpoints.size(), _color,
				_closed, _thickness);
		}


	}

	void mvDrawPolyline::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				_points = ToVectVec4(item);
				break;

			default:
				break;
			}
		}
	}

	void mvDrawPolyline::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "points")) _points = ToVectVec4(item);
		if (PyObject* item = PyDict_GetItemString(dict, "closed")) _closed = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) _thickness = ToFloat(item);

		for (auto& point : _points)
			point.w = 1.0f;
	}

	void mvDrawPolyline::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "points", mvPyObject(ToPyList(_points)));
		PyDict_SetItemString(dict, "closed", mvPyObject(ToPyBool(_closed)));
		PyDict_SetItemString(dict, "color", mvPyObject(ToPyColor(_color)));
		PyDict_SetItemString(dict, "thickness", mvPyObject(ToPyFloat(_thickness)));
	}
}