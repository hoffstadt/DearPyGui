#include "mvDrawPolyline.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvDrawPolyline::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SHOW)
		);

		args.push_back({ mvPyDataType::ListFloatList, "points" });
		args.push_back({ mvPyDataType::Bool, "closed", mvArgType::KEYWORD_ARG, "False", "Will close the polyline by returning to the first point." });
		args.push_back({ mvPyDataType::IntList, "color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)" });
		args.push_back({ mvPyDataType::Float, "thickness", mvArgType::KEYWORD_ARG, "1.0" });

		mvPythonParserSetup setup;
		setup.about = "Adds a polyline.";
		setup.category = { "Drawlist", "Widgets" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

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

		for (auto& point : points)
			point = _transform * point;

		if (_perspectiveDivide)
		{

			for (auto& point : points)
			{
				point.x = point.x / point.w;
				point.x = point.x / point.w;
				point.x = point.x / point.w;
			}
		}

		if (_depthClipping)
		{
			for(auto& point : points)
				if (mvClipPoint(_clipViewport, point)) return;
		}

		if (ImPlot::GetCurrentContext()->CurrentPlot)
		{
			for (auto& point : points)
			{
				ImVec2 impoint = ImPlot::PlotToPixels(point);
				point.x = impoint.x;
				point.y = impoint.y;
			}

			drawlist->AddPolyline((const ImVec2*)const_cast<const mvVec4*>(points.data()), (int)_points.size(), _color,
				_closed, ImPlot::GetCurrentContext()->Mx * _thickness);
		}
		else
		{
			for (auto& point : points)
				point = point + start;

			drawlist->AddPolyline((const ImVec2*)const_cast<const mvVec4*>(points.data()), (int)_points.size(), _color,
				_closed, _thickness);
		}


	}

	void mvDrawPolyline::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!VerifyRequiredArguments(GetParsers()[s_command], dict))
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