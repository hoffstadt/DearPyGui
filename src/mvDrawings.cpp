#include "mvDrawings.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPyUtils.h"
#include "mvAppItemCommons.h"
#include <math.h>
#include "mvContext.h"
#include "mvCustomTypes.h"

mvDrawArrow::mvDrawArrow(mvUUID uuid)
	:
	mvAppItem(uuid)
{
	updatePoints();
}

void mvDrawArrow::applySpecificTemplate(mvAppItem* item)
{
	auto titem = static_cast<mvDrawArrow*>(item);
	_p1 = titem->_p1;
	_p2 = titem->_p2;
	_color = titem->_color;
	_thickness = titem->_thickness;
	_size = titem->_size;
	_points[0] = titem->_points[0];
	_points[1] = titem->_points[1];
	_points[2] = titem->_points[2];
}

void mvDrawArrow::updatePoints()
{
	float xsi = _p1.x;
	float xfi = _p2.x;
	float ysi = _p1.y;
	float yfi = _p2.y;

	// length of arrow head
	double xoffset = _size;
	double yoffset = _size;

	// get pointer angle w.r.t +X (in radians)
	double angle = 0.0;
	if (xsi >= xfi && ysi >= yfi) {
		angle = atan((ysi - yfi) / (xsi - xfi));
	}
	else if (xsi < xfi && ysi >= yfi) {
		angle = M_PI + atan((ysi - yfi) / (xsi - xfi));
	}
	else if (xsi < xfi && ysi < yfi) {
		angle = -M_PI + atan((ysi - yfi) / (xsi - xfi));
	}
	else if (xsi >= xfi && ysi < yfi) {
		angle = atan((ysi - yfi) / (xsi - xfi));
	}

	// arrow head points
	auto x1 = (float)(xsi - xoffset * cos(angle));
	auto y1 = (float)(ysi - yoffset * sin(angle));

	_points[0] = { xsi, ysi, 0.0f, 1.0f };
	_points[1] = { (float)(x1 - 0.5 * _size * sin(angle)), (float)(y1 + 0.5 * _size * cos(angle)), 0.0f, 1.0f };
	_points[2] = { (float)(x1 + 0.5 * _size * cos((M_PI / 2.0) - angle)), (float)(y1 - 0.5 * _size * sin((M_PI / 2.0) - angle)), 0.0f, 1.0f };

}

void mvDrawArrow::draw(ImDrawList* drawlist, float x, float y)
{

	mvVec4  tp1 = drawInfo->transform * _p1;
	mvVec4  tp2 = drawInfo->transform * _p2;

	mvVec4  tpp1 = drawInfo->transform * _points[0];
	mvVec4  tpp2 = drawInfo->transform * _points[1];
	mvVec4  tpp3 = drawInfo->transform * _points[2];

	if (drawInfo->perspectiveDivide)
	{
		tp1.x = tp1.x / tp1.w;
		tp2.x = tp2.x / tp2.w;
		tpp1.x = tpp1.x / tpp1.w;
		tpp2.x = tpp2.x / tpp2.w;
		tpp3.x = tpp3.x / tpp3.w;

		tp1.y = tp1.y / tp1.w;
		tp2.y = tp2.y / tp2.w;
		tpp1.y = tpp1.y / tpp1.w;
		tpp2.y = tpp2.y / tpp2.w;
		tpp3.y = tpp3.y / tpp3.w;

		tp1.z = tp1.z / tp1.w;
		tp2.z = tp2.z / tp2.w;
		tpp1.z = tpp1.z / tpp1.w;
		tpp2.z = tpp2.z / tpp2.w;
		tpp3.z = tpp3.z / tpp3.w;
	}

	if (drawInfo->depthClipping)
	{
		if (mvClipPoint(drawInfo->clipViewport, tp1)) return;
		if (mvClipPoint(drawInfo->clipViewport, tp2)) return;
		if (mvClipPoint(drawInfo->clipViewport, tpp1)) return;
		if (mvClipPoint(drawInfo->clipViewport, tpp2)) return;
		if (mvClipPoint(drawInfo->clipViewport, tpp3)) return;
	}

	if (ImPlot::GetCurrentContext()->CurrentPlot)
	{
		drawlist->AddLine(ImPlot::PlotToPixels(tp1), ImPlot::PlotToPixels(tp1), _color, (float)ImPlot::GetCurrentContext()->Mx * _thickness);
		drawlist->AddTriangle(ImPlot::PlotToPixels(tpp1), ImPlot::PlotToPixels(tpp2), ImPlot::PlotToPixels(tpp3), _color, (float)ImPlot::GetCurrentContext()->Mx * _thickness);
		drawlist->AddTriangleFilled(ImPlot::PlotToPixels(tpp1), ImPlot::PlotToPixels(tpp2), ImPlot::PlotToPixels(tpp3), _color);
	}
	else
	{
		mvVec2 start = { x, y };
		drawlist->AddLine(tp1 + start, tp2 + start, _color, _thickness);
		drawlist->AddTriangle(tpp1 + start, tpp2 + start, tpp3 + start, _color, _thickness);
		drawlist->AddTriangleFilled(tpp1 + start, tpp2 + start, tpp3 + start, _color);
	}
}

void mvDrawArrow::handleSpecificRequiredArgs(PyObject* dict)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	_p1 = ToVec4(PyTuple_GetItem(dict, 0));
	_p2 = ToVec4(PyTuple_GetItem(dict, 1));
	_p1.w = 1.0f;
	_p2.w = 1.0f;

	updatePoints();
}

void mvDrawArrow::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;


	if (PyObject* item = PyDict_GetItemString(dict, "p1")) _p1 = ToVec4(item);
	if (PyObject* item = PyDict_GetItemString(dict, "p2")) _p2 = ToVec4(item);
	if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);
	if (PyObject* item = PyDict_GetItemString(dict, "thickness")) _thickness = ToFloat(item);
	if (PyObject* item = PyDict_GetItemString(dict, "size")) _size = ToFloat(item);

	_p1.w = 1.0f;
	_p2.w = 1.0f;

	updatePoints();

}

void mvDrawArrow::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;
	PyDict_SetItemString(dict, "p1", mvPyObject(ToPyPair(_p1.x, _p1.y)));
	PyDict_SetItemString(dict, "p2", mvPyObject(ToPyPair(_p2.x, _p2.y)));
	PyDict_SetItemString(dict, "color", mvPyObject(ToPyColor(_color)));
	PyDict_SetItemString(dict, "thickness", mvPyObject(ToPyFloat(_thickness)));
	PyDict_SetItemString(dict, "size", mvPyObject(ToPyFloat(_size)));
}

void mvDrawBezierCubic::applySpecificTemplate(mvAppItem* item)
{
	auto titem = static_cast<mvDrawBezierCubic*>(item);
	_p1 = titem->_p1;
	_p2 = titem->_p2;
	_p3 = titem->_p3;
	_p4 = titem->_p4;
	_color = titem->_color;
	_thickness = titem->_thickness;
	_segments = titem->_segments;
}

void mvDrawBezierCubic::draw(ImDrawList* drawlist, float x, float y)
{
	mvVec4  tp1 = drawInfo->transform * _p1;
	mvVec4  tp2 = drawInfo->transform * _p2;
	mvVec4  tp3 = drawInfo->transform * _p3;
	mvVec4  tp4 = drawInfo->transform * _p4;

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
		drawlist->AddBezierCubic(ImPlot::PlotToPixels(tp1), ImPlot::PlotToPixels(tp2), ImPlot::PlotToPixels(tp3),
			ImPlot::PlotToPixels(tp4), _color, ImPlot::GetCurrentContext()->Mx * _thickness, _segments);
	else
	{
		mvVec2 start = { x, y };
		drawlist->AddBezierCubic(tp1 + start, tp2 + start, tp3 + start, tp4 + start, _color, _thickness, _segments);
	}
}

void mvDrawBezierCubic::handleSpecificRequiredArgs(PyObject* dict)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	_p1 = ToVec4(PyTuple_GetItem(dict, 0));
	_p2 = ToVec4(PyTuple_GetItem(dict, 1));
	_p3 = ToVec4(PyTuple_GetItem(dict, 2));
	_p4 = ToVec4(PyTuple_GetItem(dict, 3));
	_p1.w = 1.0f;
	_p2.w = 1.0f;
	_p3.w = 1.0f;
	_p4.w = 1.0f;
}

void mvDrawBezierCubic::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(dict, "p1")) _p1 = ToVec4(item);
	if (PyObject* item = PyDict_GetItemString(dict, "p2")) _p2 = ToVec4(item);
	if (PyObject* item = PyDict_GetItemString(dict, "p3")) _p3 = ToVec4(item);
	if (PyObject* item = PyDict_GetItemString(dict, "p4")) _p4 = ToVec4(item);
	if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);
	if (PyObject* item = PyDict_GetItemString(dict, "thickness")) _thickness = ToFloat(item);
	if (PyObject* item = PyDict_GetItemString(dict, "segments")) _segments = ToInt(item);

	_p1.w = 1.0f;
	_p2.w = 1.0f;
	_p3.w = 1.0f;
	_p4.w = 1.0f;
}

void mvDrawBezierCubic::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;

	PyDict_SetItemString(dict, "p1", mvPyObject(ToPyPair(_p1.x, _p1.y)));
	PyDict_SetItemString(dict, "p2", mvPyObject(ToPyPair(_p2.x, _p2.y)));
	PyDict_SetItemString(dict, "p3", mvPyObject(ToPyPair(_p3.x, _p3.y)));
	PyDict_SetItemString(dict, "p4", mvPyObject(ToPyPair(_p4.x, _p4.y)));
	PyDict_SetItemString(dict, "color", mvPyObject(ToPyColor(_color)));
	PyDict_SetItemString(dict, "thickness", mvPyObject(ToPyFloat(_thickness)));
	PyDict_SetItemString(dict, "segments", mvPyObject(ToPyInt(_segments)));
}

void mvDrawBezierQuadratic::applySpecificTemplate(mvAppItem* item)
{
	auto titem = static_cast<mvDrawBezierQuadratic*>(item);
	_p1 = titem->_p1;
	_p2 = titem->_p2;
	_p3 = titem->_p3;
	_color = titem->_color;
	_thickness = titem->_thickness;
	_segments = titem->_segments;
}

void mvDrawBezierQuadratic::draw(ImDrawList* drawlist, float x, float y)
{
	mvVec4  tp1 = drawInfo->transform * _p1;
	mvVec4  tp2 = drawInfo->transform * _p2;
	mvVec4  tp3 = drawInfo->transform * _p3;

	if (drawInfo->perspectiveDivide)
	{
		tp1.x = tp1.x / tp1.w;
		tp2.x = tp2.x / tp2.w;
		tp3.x = tp3.x / tp3.w;

		tp1.y = tp1.y / tp1.w;
		tp2.y = tp2.y / tp2.w;
		tp3.y = tp3.y / tp3.w;

		tp1.z = tp1.z / tp1.w;
		tp2.z = tp2.z / tp2.w;
		tp3.z = tp3.z / tp3.w;
	}

	if (drawInfo->depthClipping)
	{
		if (mvClipPoint(drawInfo->clipViewport, tp1)) return;
		if (mvClipPoint(drawInfo->clipViewport, tp2)) return;
		if (mvClipPoint(drawInfo->clipViewport, tp3)) return;
	}

	if (ImPlot::GetCurrentContext()->CurrentPlot)
		drawlist->AddBezierQuadratic(ImPlot::PlotToPixels(tp1),
			ImPlot::PlotToPixels(tp2), ImPlot::PlotToPixels(tp3), _color, ImPlot::GetCurrentContext()->Mx * _thickness, _segments);
	else
	{
		mvVec2 start = { x, y };
		drawlist->AddBezierQuadratic(tp1 + start, tp2 + start, tp3 + start, _color, _thickness, _segments);
	}
}

void mvDrawBezierQuadratic::handleSpecificRequiredArgs(PyObject* dict)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	_p1 = ToVec4(PyTuple_GetItem(dict, 0));
	_p2 = ToVec4(PyTuple_GetItem(dict, 1));
	_p3 = ToVec4(PyTuple_GetItem(dict, 2));
	_p1.w = 1.0f;
	_p2.w = 1.0f;
	_p3.w = 1.0f;
}

void mvDrawBezierQuadratic::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(dict, "p1")) _p1 = ToVec4(item);
	if (PyObject* item = PyDict_GetItemString(dict, "p2")) _p2 = ToVec4(item);
	if (PyObject* item = PyDict_GetItemString(dict, "p3")) _p3 = ToVec4(item);
	if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);
	if (PyObject* item = PyDict_GetItemString(dict, "thickness")) _thickness = ToFloat(item);
	if (PyObject* item = PyDict_GetItemString(dict, "segments")) _segments = ToInt(item);

	_p1.w = 1.0f;
	_p2.w = 1.0f;
	_p3.w = 1.0f;
}

void mvDrawBezierQuadratic::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;

	PyDict_SetItemString(dict, "p1", mvPyObject(ToPyPair(_p1.x, _p1.y)));
	PyDict_SetItemString(dict, "p2", mvPyObject(ToPyPair(_p2.x, _p2.y)));
	PyDict_SetItemString(dict, "p3", mvPyObject(ToPyPair(_p3.x, _p3.y)));
	PyDict_SetItemString(dict, "color", mvPyObject(ToPyColor(_color)));
	PyDict_SetItemString(dict, "thickness", mvPyObject(ToPyFloat(_thickness)));
	PyDict_SetItemString(dict, "segments", mvPyObject(ToPyInt(_segments)));
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

	_center = ToVec4(PyTuple_GetItem(dict, 0));
	_center.w = 1.0f;
	_radius = ToFloat(PyTuple_GetItem(dict, 1));
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

void mvDrawEllipse::applySpecificTemplate(mvAppItem* item)
{
	auto titem = static_cast<mvDrawEllipse*>(item);
	_pmin = titem->_pmin;
	_pmax = titem->_pmax;
	_segments = titem->_segments;
	_color = titem->_color;
	_fill = titem->_fill;
	_thickness = titem->_thickness;
	_points = titem->_points;
}

void mvDrawEllipse::draw(ImDrawList* drawlist, float x, float y)
{
	mvVec4  tpmin = drawInfo->transform * _pmin;
	mvVec4  tpmax = drawInfo->transform * _pmax;

	if (drawInfo->perspectiveDivide)
	{
		tpmin.x = tpmin.x / tpmin.w;
		tpmin.y = tpmin.y / tpmin.w;
		tpmin.z = tpmin.z / tpmin.w;

		tpmax.x = tpmax.x / tpmax.w;
		tpmax.y = tpmax.y / tpmax.w;
		tpmax.z = tpmax.z / tpmax.w;
	}

	if (_dirty)
	{
		if (_segments < 3) { _segments = 3; }
		const float  width = _pmax.x - _pmin.x;
		const float  height = _pmax.y - _pmin.y;
		const float  cx = width / 2.0f + _pmin.x;
		const float  cy = height / 2.0f + _pmin.y;
		const float radian_inc = ((float)M_PI * 2.0f) / (float)_segments;
		std::vector<mvVec4> points;
		points.reserve(_segments + 1);
		for (int i = 0; i <= _segments; i++)
		{
			points.push_back(mvVec4{ cx + cosf(i * radian_inc) * width / 2.0f, cy + sinf(i * radian_inc) * height / 2.0f, 0.0f, 1.0f });
		}
		_points = std::move(points);
		_dirty = false;
	}

	if (drawInfo->depthClipping)
	{
		if (mvClipPoint(drawInfo->clipViewport, tpmin)) return;
		if (mvClipPoint(drawInfo->clipViewport, tpmax)) return;
	}

	// this is disgusting; we should not be allocating
	// every frame. Fix ASAP
	std::vector<mvVec4> points = _points;
	std::vector<ImVec2> finalpoints;
	finalpoints.reserve(_points.size());

	for (auto& point : points)
		point = drawInfo->transform * point;

	if (ImPlot::GetCurrentContext()->CurrentPlot)
	{
		for (auto& point : points)
		{
			ImVec2 impoint = ImPlot::PlotToPixels(point);
			point.x = impoint.x;
			point.y = impoint.y;
			finalpoints.push_back(impoint);
		}
	}
	else
	{
		for (auto& point : points)
		{
			point.x += x;
			point.y += y;
			finalpoints.push_back(ImVec2{ point.x, point.y });
		}
	}

	if (ImPlot::GetCurrentContext()->CurrentPlot)
		drawlist->AddPolyline(finalpoints.data(), (int)finalpoints.size(),
			_color, false, ImPlot::GetCurrentContext()->Mx * _thickness);
	else
		drawlist->AddPolyline(finalpoints.data(), (int)finalpoints.size(),
			_color, false, _thickness);
	if (_fill.r < 0.0f)
		return;
	drawlist->AddConvexPolyFilled(finalpoints.data(), (int)finalpoints.size(), _fill);
}

void mvDrawEllipse::handleSpecificRequiredArgs(PyObject* dict)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	_pmin = ToVec4(PyTuple_GetItem(dict, 0));
	_pmin.w = 1.0f;
	_pmax = ToVec4(PyTuple_GetItem(dict, 1));
	_pmax.w = 1.0f;
	_dirty = true;
}

void mvDrawEllipse::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;


	if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);
	if (PyObject* item = PyDict_GetItemString(dict, "fill")) _fill = ToColor(item);
	if (PyObject* item = PyDict_GetItemString(dict, "thickness")) _thickness = ToFloat(item);
	if (PyObject* item = PyDict_GetItemString(dict, "pmax")) { _pmax = ToVec4(item); _dirty = true; }
	if (PyObject* item = PyDict_GetItemString(dict, "pmin")) { _pmin = ToVec4(item); _dirty = true; }
	if (PyObject* item = PyDict_GetItemString(dict, "segments")) _segments = ToInt(item);

	_pmin.w = 1.0f;
	_pmax.w = 1.0f;
}

void mvDrawEllipse::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;

	PyDict_SetItemString(dict, "color", mvPyObject(ToPyColor(_color)));
	PyDict_SetItemString(dict, "fill", mvPyObject(ToPyColor(_fill)));
	PyDict_SetItemString(dict, "thickness", mvPyObject(ToPyFloat(_thickness)));
	PyDict_SetItemString(dict, "pmax", mvPyObject(ToPyPair(_pmax.x, _pmax.y)));
	PyDict_SetItemString(dict, "pmin", mvPyObject(ToPyPair(_pmin.x, _pmin.y)));
	PyDict_SetItemString(dict, "segments", mvPyObject(ToPyInt(_segments)));
}

void mvDrawImage::applySpecificTemplate(mvAppItem* item)
{
	auto titem = static_cast<mvDrawImage*>(item);
	_textureUUID = titem->_textureUUID;
	_pmax = titem->_pmax;
	_pmin = titem->_pmin;
	_uv_min = titem->_uv_min;
	_uv_max = titem->_uv_max;
	_color = titem->_color;
	_texture = titem->_texture;
	_internalTexture = titem->_internalTexture;
}

void mvDrawImage::draw(ImDrawList* drawlist, float x, float y)
{
	if (_texture)
	{
		if (_internalTexture)
			_texture->draw(drawlist, x, y);

		if (!_texture->state.ok)
			return;

		void* texture = nullptr;

		if (_texture->type == mvAppItemType::mvStaticTexture)
			texture = static_cast<mvStaticTexture*>(_texture.get())->_texture;
		else if (_texture->type == mvAppItemType::mvRawTexture)
			texture = static_cast<mvRawTexture*>(_texture.get())->_texture;
		else
			texture = static_cast<mvDynamicTexture*>(_texture.get())->_texture;

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
			drawlist->AddImage(texture, ImPlot::PlotToPixels(tpmin), ImPlot::PlotToPixels(tpmax), _uv_min, _uv_max, _color);
		else
		{
			mvVec2 start = { x, y };
			drawlist->AddImage(texture, tpmin + start, tpmax + start, _uv_min, _uv_max, _color);
		}
	}
}

void mvDrawImage::handleSpecificRequiredArgs(PyObject* dict)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	_textureUUID = GetIDFromPyObject(PyTuple_GetItem(dict, 0));
	_texture = GetRefItem(*GContext->itemRegistry, _textureUUID);
	if (_texture)
	{
		_pmin = ToVec4(PyTuple_GetItem(dict, 1));
		_pmin.w = 1.0f;
		_pmax = ToVec4(PyTuple_GetItem(dict, 2));
		_pmax.w = 1.0f;
	}
	else if (_textureUUID == MV_ATLAS_UUID)
	{
		_texture = std::make_shared<mvStaticTexture>(_textureUUID);
		_internalTexture = true;
	}
	else
		mvThrowPythonError(mvErrorCode::mvTextureNotFound, GetEntityCommand(type), "Texture not found.", this);

	_pmin = ToVec4(PyTuple_GetItem(dict, 1));
	_pmin.w = 1.0f;
	_pmax = ToVec4(PyTuple_GetItem(dict, 2));
	_pmax.w = 1.0f;
}

void mvDrawImage::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(dict, "pmax")) _pmax = ToVec4(item);
	if (PyObject* item = PyDict_GetItemString(dict, "pmin")) _pmin = ToVec4(item);
	if (PyObject* item = PyDict_GetItemString(dict, "uv_min")) _uv_min = ToVec2(item);
	if (PyObject* item = PyDict_GetItemString(dict, "uv_max")) _uv_max = ToVec2(item);
	if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);
	if (PyObject* item = PyDict_GetItemString(dict, "texture_tag"))
	{
		_textureUUID = GetIDFromPyObject(item);
		_texture = GetRefItem(*GContext->itemRegistry, _textureUUID);
		if (_textureUUID == MV_ATLAS_UUID)
		{
			_texture = std::make_shared<mvStaticTexture>(_textureUUID);
			_internalTexture = true;
		}
		else if (_texture)
		{
			_internalTexture = false;
		}
		else
		{
			mvThrowPythonError(mvErrorCode::mvTextureNotFound, GetEntityCommand(type), "Texture not found.", this);
		}
	}

	_pmin.w = 1.0f;
	_pmax.w = 1.0f;
}

void mvDrawImage::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;

	PyDict_SetItemString(dict, "pmax", mvPyObject(ToPyPair(_pmax.x, _pmax.y)));
	PyDict_SetItemString(dict, "pmin", mvPyObject(ToPyPair(_pmin.x, _pmin.y)));
	PyDict_SetItemString(dict, "uv_min", mvPyObject(ToPyPair(_uv_min.x, _uv_min.y)));
	PyDict_SetItemString(dict, "uv_max", mvPyObject(ToPyPair(_uv_max.x, _uv_max.y)));
	PyDict_SetItemString(dict, "color", mvPyObject(ToPyColor(_color)));
	PyDict_SetItemString(dict, "texture_tag", mvPyObject(ToPyUUID(_textureUUID)));
}

void mvDrawImageQuad::applySpecificTemplate(mvAppItem* item)
{
	auto titem = static_cast<mvDrawImageQuad*>(item);
	_textureUUID = titem->_textureUUID;
	_p1 = titem->_p1;
	_p2 = titem->_p2;
	_p3 = titem->_p3;
	_p4 = titem->_p4;
	_uv1 = titem->_uv1;
	_uv2 = titem->_uv2;
	_uv3 = titem->_uv3;
	_uv4 = titem->_uv4;
	_color = titem->_color;
	_texture = titem->_texture;
	_internalTexture = titem->_internalTexture;
}

void mvDrawImageQuad::draw(ImDrawList* drawlist, float x, float y)
{
	if (_texture)
	{
		if (_internalTexture)
			_texture->draw(drawlist, x, y);

		if (!_texture->state.ok)
			return;

		void* texture = nullptr;

		if (_texture->type == mvAppItemType::mvStaticTexture)
			texture = static_cast<mvStaticTexture*>(_texture.get())->_texture;
		else if (_texture->type == mvAppItemType::mvRawTexture)
			texture = static_cast<mvRawTexture*>(_texture.get())->_texture;
		else
			texture = static_cast<mvDynamicTexture*>(_texture.get())->_texture;

		mvVec4  tp1 = drawInfo->transform * _p1;
		mvVec4  tp2 = drawInfo->transform * _p2;
		mvVec4  tp3 = drawInfo->transform * _p3;
		mvVec4  tp4 = drawInfo->transform * _p4;

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
			drawlist->AddImageQuad(texture, ImPlot::PlotToPixels(tp1),
				ImPlot::PlotToPixels(tp2), ImPlot::PlotToPixels(tp3), ImPlot::PlotToPixels(tp4),
				_uv1, _uv2, _uv3, _uv4, _color);
		else
		{
			mvVec2 start = { x, y };
			drawlist->AddImageQuad(texture, tp1 + start, tp2 + start, tp3.xy(), tp4.xy(), _uv1, _uv2, _uv3, _uv4, _color);
		}
	}
}

void mvDrawImageQuad::handleSpecificRequiredArgs(PyObject* dict)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	_textureUUID = GetIDFromPyObject(PyTuple_GetItem(dict, 0));
	_texture = GetRefItem(*GContext->itemRegistry, _textureUUID);
	if (_texture)
	{
		_p1 = ToVec4(PyTuple_GetItem(dict, 1));
		_p1.w = 1.0f;
		_p2 = ToVec4(PyTuple_GetItem(dict, 2));
		_p2.w = 1.0f;
		_p3 = ToVec4(PyTuple_GetItem(dict, 3));
		_p3.w = 1.0f;
		_p4 = ToVec4(PyTuple_GetItem(dict, 4));
		_p4.w = 1.0f;
	}
	else if (_textureUUID == MV_ATLAS_UUID)
	{
		_texture = std::make_shared<mvStaticTexture>(_textureUUID);
		_internalTexture = true;
	}
	else
		mvThrowPythonError(mvErrorCode::mvTextureNotFound, GetEntityCommand(type), "Texture not found.", this);

	_p1 = ToVec4(PyTuple_GetItem(dict, 1));
	_p1.w = 1.0f;
	_p2 = ToVec4(PyTuple_GetItem(dict, 2));
	_p2.w = 1.0f;
	_p3 = ToVec4(PyTuple_GetItem(dict, 3));
	_p3.w = 1.0f;
	_p4 = ToVec4(PyTuple_GetItem(dict, 4));
	_p4.w = 1.0f;

}

void mvDrawImageQuad::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(dict, "p1")) _p1 = ToVec4(item);
	if (PyObject* item = PyDict_GetItemString(dict, "p2")) _p2 = ToVec4(item);
	if (PyObject* item = PyDict_GetItemString(dict, "p3")) _p3 = ToVec4(item);
	if (PyObject* item = PyDict_GetItemString(dict, "p4")) _p4 = ToVec4(item);
	if (PyObject* item = PyDict_GetItemString(dict, "uv1")) _uv1 = ToVec2(item);
	if (PyObject* item = PyDict_GetItemString(dict, "uv2")) _uv2 = ToVec2(item);
	if (PyObject* item = PyDict_GetItemString(dict, "uv3")) _uv3 = ToVec2(item);
	if (PyObject* item = PyDict_GetItemString(dict, "uv4")) _uv4 = ToVec2(item);
	if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);
	if (PyObject* item = PyDict_GetItemString(dict, "texture_tag"))
	{
		_textureUUID = GetIDFromPyObject(item);
		_texture = GetRefItem(*GContext->itemRegistry, _textureUUID);
		if (_textureUUID == MV_ATLAS_UUID)
		{
			_texture = std::make_shared<mvStaticTexture>(_textureUUID);
			_internalTexture = true;
		}
		else if (_texture)
		{
			_internalTexture = false;
		}
		else
		{
			mvThrowPythonError(mvErrorCode::mvTextureNotFound, GetEntityCommand(type), "Texture not found.", this);
		}
	}

	_p1.w = 1.0f;
	_p2.w = 1.0f;
	_p3.w = 1.0f;
	_p4.w = 1.0f;
}

void mvDrawImageQuad::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;

	PyDict_SetItemString(dict, "p1", mvPyObject(ToPyPair(_p1.x, _p1.y)));
	PyDict_SetItemString(dict, "p2", mvPyObject(ToPyPair(_p2.x, _p2.y)));
	PyDict_SetItemString(dict, "p3", mvPyObject(ToPyPair(_p3.x, _p3.y)));
	PyDict_SetItemString(dict, "p4", mvPyObject(ToPyPair(_p4.x, _p4.y)));
	PyDict_SetItemString(dict, "uv1", mvPyObject(ToPyPair(_uv1.x, _uv1.y)));
	PyDict_SetItemString(dict, "uv2", mvPyObject(ToPyPair(_uv2.x, _uv2.y)));
	PyDict_SetItemString(dict, "uv3", mvPyObject(ToPyPair(_uv3.x, _uv3.y)));
	PyDict_SetItemString(dict, "uv4", mvPyObject(ToPyPair(_uv4.x, _uv4.y)));
	PyDict_SetItemString(dict, "color", mvPyObject(ToPyColor(_color)));
	PyDict_SetItemString(dict, "texture_tag", mvPyObject(ToPyUUID(_textureUUID)));
}

mvDrawLayer::~mvDrawLayer()
{
}

void mvDrawLayer::draw(ImDrawList* drawlist, float x, float y)
{

	for (auto& item : childslots[2])
	{
		// skip item if it's not shown
		if (!item->config.show)
			continue;

		item->drawInfo->perspectiveDivide = drawInfo->perspectiveDivide;
		item->drawInfo->depthClipping = drawInfo->depthClipping;

		item->drawInfo->transform = drawInfo->transform;

		item->drawInfo->cullMode = drawInfo->cullMode;
		item->drawInfo->clipViewport[0] = drawInfo->clipViewport[0];
		item->drawInfo->clipViewport[1] = drawInfo->clipViewport[1];
		item->drawInfo->clipViewport[2] = drawInfo->clipViewport[2];
		item->drawInfo->clipViewport[3] = drawInfo->clipViewport[3];
		item->drawInfo->clipViewport[4] = drawInfo->clipViewport[4];
		item->drawInfo->clipViewport[5] = drawInfo->clipViewport[5];
		item->draw(drawlist, x, y);

		UpdateAppItemState(item->state);
	}
}

void mvDrawLayer::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(dict, "perspective_divide")) drawInfo->perspectiveDivide = ToBool(item);
	if (PyObject* item = PyDict_GetItemString(dict, "depth_clipping")) drawInfo->depthClipping = ToBool(item);
	if (PyObject* item = PyDict_GetItemString(dict, "cull_mode")) drawInfo->cullMode = ToInt(item);

}

void mvDrawLayer::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;
	PyDict_SetItemString(dict, "perspective_divide", mvPyObject(ToPyBool(drawInfo->perspectiveDivide)));
	PyDict_SetItemString(dict, "depth_clipping", mvPyObject(ToPyBool(drawInfo->depthClipping)));
	PyDict_SetItemString(dict, "cull_mode", mvPyObject(ToPyInt(drawInfo->cullMode)));
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

	if (ImPlot::GetCurrentContext()->CurrentPlot)
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

	_p1 = ToVec4(PyTuple_GetItem(dict, 0));
	_p2 = ToVec4(PyTuple_GetItem(dict, 1));
	_p1.w = 1.0f;
	_p2.w = 1.0f;

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

void mvDrawlist::applySpecificTemplate(mvAppItem* item)
{
	auto titem = static_cast<mvDrawlist*>(item);
	config.width = titem->config.width;
	config.height = titem->config.height;
}

void mvDrawlist::draw(ImDrawList* drawlist, float x, float y)
{

	if (!config.show)
		return;

	_startx = (float)ImGui::GetCursorScreenPos().x;
	_starty = (float)ImGui::GetCursorScreenPos().y;

	ImDrawList* internal_drawlist = ImGui::GetWindowDrawList();

	ImGui::PushClipRect({ _startx, _starty }, { _startx + (float)config.width, _starty + (float)config.height }, true);

	for (auto& item : childslots[2])
	{
		// skip item if it's not shown
		if (!item->config.show)
			continue;

		item->draw(internal_drawlist, _startx, _starty);

		UpdateAppItemState(item->state);
	}

	ImGui::PopClipRect();

	if (ImGui::InvisibleButton(info.internalLabel.c_str(), ImVec2((float)config.width, (float)config.height), ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight | ImGuiButtonFlags_MouseButtonMiddle))
	{
		if (config.alias.empty())
			mvAddCallback(getCallback(false), uuid, nullptr, config.user_data);
		else
			mvAddCallback(getCallback(false), config.alias, nullptr, config.user_data);
	}

	UpdateAppItemState(state);

	if (handlerRegistry)
		handlerRegistry->checkEvents(&state);

	if (ImGui::IsItemHovered())
	{
		ImVec2 mousepos = ImGui::GetMousePos();
		GContext->input.mouseDrawingPos.x = (int)(mousepos.x - _startx);
		GContext->input.mouseDrawingPos.y = (int)(mousepos.y - _starty);
	}
}

void mvDrawlist::handleSpecificRequiredArgs(PyObject* dict)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	config.width = ToInt(PyTuple_GetItem(dict, 0));
	config.height = ToInt(PyTuple_GetItem(dict, 1));
}

void mvDrawlist::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;

	PyDict_SetItemString(dict, "width", mvPyObject(ToPyInt(config.width)));
	PyDict_SetItemString(dict, "height", mvPyObject(ToPyInt(config.height)));
}

mvDrawNode::~mvDrawNode()
{
}

void mvDrawNode::draw(ImDrawList* drawlist, float x, float y)
{

	for (auto& item : childslots[2])
	{
		// skip item if it's not shown
		if (!item->config.show)
			continue;

		item->drawInfo->transform = drawInfo->transform * drawInfo->appliedTransform;

		item->drawInfo->perspectiveDivide = drawInfo->perspectiveDivide;
		item->drawInfo->depthClipping = drawInfo->depthClipping;
		item->drawInfo->cullMode = drawInfo->cullMode;
		item->drawInfo->clipViewport[0] = drawInfo->clipViewport[0];
		item->drawInfo->clipViewport[1] = drawInfo->clipViewport[1];
		item->drawInfo->clipViewport[2] = drawInfo->clipViewport[2];
		item->drawInfo->clipViewport[3] = drawInfo->clipViewport[3];
		item->drawInfo->clipViewport[4] = drawInfo->clipViewport[4];
		item->drawInfo->clipViewport[5] = drawInfo->clipViewport[5];
		item->draw(drawlist, x, y);

		UpdateAppItemState(item->state);
	}
}

void mvDrawNode::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;

}

void mvDrawNode::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;
}

void mvDrawPolygon::applySpecificTemplate(mvAppItem* item)
{
	auto titem = static_cast<mvDrawPolygon*>(item);
	_color = titem->_color;
	_fill = titem->_fill;
	_thickness = titem->_thickness;
	_points = titem->_points;
}

void mvDrawPolygon::draw(ImDrawList* drawlist, float x, float y)
{
	mvVec4 start = { x, y };

	// this is disgusting; we should not be allocating
	// every frame. Fix ASAP
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
			point.y = point.y / point.w;
			point.z = point.z / point.w;
		}
	}

	if (drawInfo->depthClipping)
	{
		for (auto& point : points)
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
	}
	else
	{
		for (auto& point : points)
		{
			point = point + start;
			finalpoints.push_back(ImVec2{ point.x, point.y });
		}
	}
	// TODO: Find a way to store lines and only calc new fill lines when dirty similar to ellipse
	drawlist->AddPolyline(finalpoints.data(), (int)finalpoints.size(), _color, false, _thickness);
	if (_fill.r < 0.0f)
		return;

	{
		size_t i;
		int y;
		int miny, maxy;
		int x1, y1;
		int x2, y2;
		int ind1, ind2;
		size_t ints;
		size_t n = finalpoints.size();
		int* polyints = new int[n];

		/* Determine Y maxima */
		miny = (int)finalpoints[0].y;
		maxy = (int)finalpoints[0].y;
		for (i = 1; i < n; i++)
		{
			miny = std::min(miny, (int)finalpoints[i].y);
			maxy = std::max(maxy, (int)finalpoints[i].y);
		}

		/* Draw, scanning y */
		for (y = miny; y < maxy; y++) {
			ints = 0;
			for (i = 0; (i < n); i++) {
				if (!i)
				{
					ind1 = (int)n - 1;
					ind2 = 0;
				}
				else
				{
					ind1 = (int)i - 1;
					ind2 = (int)i;
				}
				y1 = (int)finalpoints[ind1].y;
				y2 = (int)finalpoints[ind2].y;
				if (y1 < y2)
				{
					x1 = (int)finalpoints[ind1].x;
					x2 = (int)finalpoints[ind2].x;
				}
				else if (y1 > y2)
				{
					y2 = (int)finalpoints[ind1].y;
					y1 = (int)finalpoints[ind2].y;
					x2 = (int)finalpoints[ind1].x;
					x1 = (int)finalpoints[ind2].x;
				}
				else
					continue;

				if (((y >= y1) && (y < y2)) || ((y == maxy) && (y > y1) && (y <= y2)))
					polyints[ints++] = (y - y1) * (x2 - x1) / (y2 - y1) + x1;

			}

			auto compare_int = [](const void* a, const void* b)
			{
				return (*(const int*)a) - (*(const int*)b);
			};

			qsort(polyints, ints, sizeof(int), compare_int);

			for (i = 0; i < ints; i += 2)
			{
				if (ImPlot::GetCurrentContext()->CurrentPlot)
					drawlist->AddLine(ImPlot::PlotToPixels({ (float)polyints[i], (float)y }),
						ImPlot::PlotToPixels({ (float)polyints[i + 1], (float)y }), _fill, ImPlot::GetCurrentContext()->Mx * _thickness);
				else
				{
					drawlist->AddLine({ (float)polyints[i], (float)y },
						{ (float)polyints[i + 1], (float)y }, _fill, _thickness);
				}
			}
		}
		delete[] polyints;
	}
}

void mvDrawPolygon::handleSpecificRequiredArgs(PyObject* dict)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	_points = ToVectVec4(PyTuple_GetItem(dict, 0));
	for (auto& point : _points)
		point.w = 1.0f;
}

void mvDrawPolygon::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(dict, "points")) _points = ToVectVec4(item);
	if (PyObject* item = PyDict_GetItemString(dict, "fill")) _fill = ToColor(item);
	if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);
	if (PyObject* item = PyDict_GetItemString(dict, "thickness")) _thickness = ToFloat(item);

	for (auto& point : _points)
		point.w = 1.0f;

}

void mvDrawPolygon::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;

	PyDict_SetItemString(dict, "points", mvPyObject(ToPyList(_points)));
	PyDict_SetItemString(dict, "fill", mvPyObject(ToPyColor(_fill)));
	PyDict_SetItemString(dict, "color", mvPyObject(ToPyColor(_color)));
	PyDict_SetItemString(dict, "thickness", mvPyObject(ToPyFloat(_thickness)));

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
		for (auto& point : points)
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

	_points = ToVectVec4(PyTuple_GetItem(dict, 0));
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

	mvVec4  tp1 = drawInfo->transform * _p1;
	mvVec4  tp2 = drawInfo->transform * _p2;
	mvVec4  tp3 = drawInfo->transform * _p3;
	mvVec4  tp4 = drawInfo->transform * _p4;

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

	_p1 = ToVec4(PyTuple_GetItem(dict, 0));
	_p2 = ToVec4(PyTuple_GetItem(dict, 1));
	_p3 = ToVec4(PyTuple_GetItem(dict, 2));
	_p4 = ToVec4(PyTuple_GetItem(dict, 3));
	_p1.w = 1.0f;
	_p2.w = 1.0f;
	_p3.w = 1.0f;
	_p4.w = 1.0f;
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

	_pmin = ToVec4(PyTuple_GetItem(dict, 0));
	_pmax = ToVec4(PyTuple_GetItem(dict, 1));
	_pmin.w = 1.0f;
	_pmax.w = 1.0f;
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

void mvDrawText::applySpecificTemplate(mvAppItem* item)
{
	auto titem = static_cast<mvDrawText*>(item);
	_pos = titem->_pos;
	_text = titem->_text;
	_color = titem->_color;
	_size = titem->_size;
}

void mvDrawText::draw(ImDrawList* drawlist, float x, float y)
{
	mvVec4  tpos = drawInfo->transform * _pos;

	if (drawInfo->perspectiveDivide)
	{
		tpos.x = tpos.x / tpos.w;
		tpos.y = tpos.y / tpos.w;
		tpos.z = tpos.z / tpos.w;
	}

	if (drawInfo->depthClipping)
	{
		if (mvClipPoint(drawInfo->clipViewport, tpos)) return;
	}

	ImFont* fontptr = ImGui::GetFont();
	if (font)
		fontptr = static_cast<mvFont*>(font.get())->getFontPtr();

	if (ImPlot::GetCurrentContext()->CurrentPlot)
		drawlist->AddText(fontptr, ImPlot::GetCurrentContext()->Mx * (float)_size, ImPlot::PlotToPixels(tpos), _color, _text.c_str());
	else
	{
		mvVec2 start = { x, y };
		drawlist->AddText(fontptr, (float)_size, tpos + start, _color, _text.c_str());
	}
}

void mvDrawText::handleSpecificRequiredArgs(PyObject* dict)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	_pos = ToVec4(PyTuple_GetItem(dict, 0));
	_pos.w = 1.0f;
	_text = ToString(PyTuple_GetItem(dict, 1));

}

void mvDrawText::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(dict, "text")) _text = ToString(item);
	if (PyObject* item = PyDict_GetItemString(dict, "pos")) _pos = ToVec4(item);
	if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);
	if (PyObject* item = PyDict_GetItemString(dict, "size")) _size = ToFloat(item);
	_pos.w = 1.0f;

}

void mvDrawText::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;

	PyDict_SetItemString(dict, "text", mvPyObject(ToPyString(_text)));
	PyDict_SetItemString(dict, "pos", mvPyObject(ToPyPair(_pos.x, _pos.y)));
	PyDict_SetItemString(dict, "color", mvPyObject(ToPyColor(_color)));
	PyDict_SetItemString(dict, "size", mvPyObject(ToPyFloat(_size)));
}

void mvDrawTriangle::applySpecificTemplate(mvAppItem* item)
{
	auto titem = static_cast<mvDrawTriangle*>(item);
	_p1 = titem->_p1;
	_p2 = titem->_p2;
	_p3 = titem->_p3;
	_color = titem->_color;
	_thickness = titem->_thickness;
	_fill = titem->_fill;
}

void mvDrawTriangle::draw(ImDrawList* drawlist, float x, float y)
{
	mvVec4  tp1 = drawInfo->transform * _p1;
	mvVec4  tp2 = drawInfo->transform * _p2;
	mvVec4  tp3 = drawInfo->transform * _p3;

	if (drawInfo->perspectiveDivide)
	{
		tp1.x = tp1.x / tp1.w;
		tp2.x = tp2.x / tp2.w;
		tp3.x = tp3.x / tp3.w;

		tp1.y = tp1.y / tp1.w;
		tp2.y = tp2.y / tp2.w;
		tp3.y = tp3.y / tp3.w;

		tp1.z = tp1.z / tp1.w;
		tp2.z = tp2.z / tp2.w;
		tp3.z = tp3.z / tp3.w;
	}

	if (drawInfo->depthClipping)
	{
		if (mvClipPoint(drawInfo->clipViewport, tp1)) return;
		if (mvClipPoint(drawInfo->clipViewport, tp2)) return;
		if (mvClipPoint(drawInfo->clipViewport, tp3)) return;
	}

	if (drawInfo->cullMode == 1) // backface
	{
		mvVec3 n = mvCross(tp2 - tp1, tp3 - tp2);

		if (n.z < 0)
			return;
	}

	else if (drawInfo->cullMode == 2) // frontface
	{
		mvVec3 n = mvCross(tp2 - tp1, tp3 - tp2);

		if (n.z > 0)
			return;
	}

	if (ImPlot::GetCurrentContext()->CurrentPlot)
	{
		drawlist->AddTriangle(ImPlot::PlotToPixels(tp1), ImPlot::PlotToPixels(tp2), ImPlot::PlotToPixels(tp3),
			_color, ImPlot::GetCurrentContext()->Mx * _thickness);
		if (_fill.r < 0.0f)
			return;
		drawlist->AddTriangleFilled(ImPlot::PlotToPixels(tp1), ImPlot::PlotToPixels(tp2), ImPlot::PlotToPixels(tp3),
			_fill);
	}
	else
	{
		mvVec2 start = { x, y };
		drawlist->AddTriangle(tp1 + start, tp2 + start, tp3 + start, _color, _thickness);
		if (_fill.r < 0.0f)
			return;
		drawlist->AddTriangleFilled(tp1 + start, tp2 + start, tp3 + start, _fill);
	}
}

void mvDrawTriangle::handleSpecificRequiredArgs(PyObject* dict)
{
	if (!VerifyRequiredArguments(GetParsers()[GetEntityCommand(type)], dict))
		return;

	_p1 = ToVec4(PyTuple_GetItem(dict, 0));
	_p2 = ToVec4(PyTuple_GetItem(dict, 1));
	_p3 = ToVec4(PyTuple_GetItem(dict, 2));
	_p1.w = 1.0f;
	_p2.w = 1.0f;
	_p3.w = 1.0f;
}

void mvDrawTriangle::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(dict, "p1")) _p1 = ToVec4(item);
	if (PyObject* item = PyDict_GetItemString(dict, "p2")) _p2 = ToVec4(item);
	if (PyObject* item = PyDict_GetItemString(dict, "p3")) _p3 = ToVec4(item);
	if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);
	if (PyObject* item = PyDict_GetItemString(dict, "fill")) _fill = ToColor(item);
	if (PyObject* item = PyDict_GetItemString(dict, "thickness")) _thickness = ToFloat(item);
	if (PyObject* item = PyDict_GetItemString(dict, "cull_mode")) drawInfo->cullMode = ToInt(item);

	_p1.w = 1.0f;
	_p2.w = 1.0f;
	_p3.w = 1.0f;
}

void mvDrawTriangle::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;

	PyDict_SetItemString(dict, "p1", mvPyObject(ToPyPair(_p1.x, _p1.y)));
	PyDict_SetItemString(dict, "p2", mvPyObject(ToPyPair(_p2.x, _p2.y)));
	PyDict_SetItemString(dict, "p3", mvPyObject(ToPyPair(_p3.x, _p3.y)));
	PyDict_SetItemString(dict, "color", mvPyObject(ToPyColor(_color)));
	PyDict_SetItemString(dict, "fill", mvPyObject(ToPyColor(_fill)));
	PyDict_SetItemString(dict, "thickness", mvPyObject(ToPyFloat(_thickness)));
}

void mvViewportDrawlist::applySpecificTemplate(mvAppItem* item)
{
	auto titem = static_cast<mvViewportDrawlist*>(item);
	_front = titem->_front;
}

void mvViewportDrawlist::draw(ImDrawList* drawlist, float x, float y)
{

	ImDrawList* internal_drawlist = _front ? ImGui::GetForegroundDrawList() : ImGui::GetBackgroundDrawList();

	for (auto& item : childslots[2])
	{
		// skip item if it's not shown
		if (!item->config.show)
			continue;

		item->draw(internal_drawlist, 0.0f, 0.0f);

		UpdateAppItemState(item->state);
	}

}

void mvViewportDrawlist::handleSpecificKeywordArgs(PyObject* dict)
{
	if (dict == nullptr)
		return;

	if (PyObject* item = PyDict_GetItemString(dict, "front")) _front = ToBool(item);

}

void mvViewportDrawlist::getSpecificConfiguration(PyObject* dict)
{
	if (dict == nullptr)
		return;

	PyDict_SetItemString(dict, "front", mvPyObject(ToPyBool(_front)));
}