#include "mvDrawCircle.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvDrawCircle::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SHOW)
		);

		args.push_back({ mvPyDataType::FloatList, "center" });
		args.push_back({ mvPyDataType::Float, "radius" });
		args.push_back({ mvPyDataType::IntList, "color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)" });
		args.push_back({ mvPyDataType::IntList, "fill", mvArgType::KEYWORD_ARG, "(0, 0, 0, -255)" });
		args.push_back({ mvPyDataType::Float, "thickness", mvArgType::KEYWORD_ARG, "1.0" });
		args.push_back({ mvPyDataType::Integer, "segments", mvArgType::KEYWORD_ARG, "0", "Number of segments to approximate circle." });

		mvPythonParserSetup setup;
		setup.about = "Adds a circle";
		setup.category = { "Drawlist", "Widgets" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

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
		mvVec4  tcenter = _transform * _center;

		if (_perspectiveDivide)
		{
			tcenter.x = tcenter.x / tcenter.w;
			tcenter.y = tcenter.y / tcenter.w;
			tcenter.z = tcenter.z / tcenter.w;
		}

		if (_depthClipping)
		{
			if (mvClipPoint(_clipViewport, tcenter)) return;
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
		if (!VerifyRequiredArguments(GetParsers()[s_command], dict))
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