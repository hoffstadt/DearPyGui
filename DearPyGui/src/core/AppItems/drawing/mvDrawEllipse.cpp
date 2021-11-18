#include "mvDrawEllipse.h"
#include "mvDrawPolygon.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include <math.h>

namespace Marvel {

	void mvDrawEllipse::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SHOW)
		);

		args.push_back({ mvPyDataType::FloatList, "pmin", mvArgType::REQUIRED_ARG, "...", "Min point of bounding rectangle."});
		args.push_back({ mvPyDataType::FloatList, "pmax", mvArgType::REQUIRED_ARG, "...", "Max point of bounding rectangle."});
		args.push_back({ mvPyDataType::IntList, "color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)" });
		args.push_back({ mvPyDataType::IntList, "fill", mvArgType::KEYWORD_ARG, "(0, 0, 0, -255)" });
		args.push_back({ mvPyDataType::Float, "thickness", mvArgType::KEYWORD_ARG, "1.0" });
		args.push_back({ mvPyDataType::Integer, "segments", mvArgType::KEYWORD_ARG, "32", "Number of segments to approximate bezier curve." });

		mvPythonParserSetup setup;
		setup.about = "Adds an ellipse.";
		setup.category = { "Drawlist", "Widgets" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvDrawEllipse::mvDrawEllipse(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
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
		mvVec4  tpmin = _transform * _pmin;
		mvVec4  tpmax = _transform * _pmax;

		if (_perspectiveDivide)
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
				points.push_back(mvVec4{ cx  + cosf(i*radian_inc) * width/2.0f, cy + sinf(i * radian_inc) * height/2.0f, 0.0f, 1.0f });
			}
			_points = std::move(points);
			_dirty = false;
		}

		if (_depthClipping)
		{
			if (mvClipPoint(_clipViewport, tpmin)) return;
			if (mvClipPoint(_clipViewport, tpmax)) return;
		}

		// this is disgusting; we should not be allocating
		// every frame. Fix ASAP
		std::vector<mvVec4> points = _points;
		std::vector<ImVec2> finalpoints;
		finalpoints.reserve(_points.size());

		for(auto& point : points)
			point = _transform * point;

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

        if(ImPlot::GetCurrentContext()->CurrentPlot)
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
		if (!VerifyRequiredArguments(GetParsers()[s_command], dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				_pmin = ToVec4(item);
				_pmin.w = 1.0f;
				_dirty = true;
				break;

			case 1:
				_pmax = ToVec4(item);
				_pmax.w = 1.0f;
				_dirty = true;
				break;

			default:
				break;
			}
		}
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

}
