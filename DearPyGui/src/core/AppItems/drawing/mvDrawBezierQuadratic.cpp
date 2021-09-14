#include "mvDrawBezierQuadratic.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvDrawBezierQuadratic::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		std::vector<mvPythonDataElement> args;

		AddCommonArgs(args,(CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SHOW)
		);

		args.push_back({ mvPyDataType::FloatList, "p1", mvArgType::REQUIRED_ARG, "...", "First point in curve." });
		args.push_back({ mvPyDataType::FloatList, "p2", mvArgType::REQUIRED_ARG, "...", "Second point in curve." });
		args.push_back({ mvPyDataType::FloatList, "p3", mvArgType::REQUIRED_ARG, "...", "Third point in curve." });
		args.push_back({ mvPyDataType::IntList, "color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)" });
		args.push_back({ mvPyDataType::Float, "thickness", mvArgType::KEYWORD_ARG, "1.0" });
		args.push_back({ mvPyDataType::Integer, "segments", mvArgType::KEYWORD_ARG, "0", "Number of segments to approximate bezier curve." });

		mvPythonParserSetup setup;
		setup.about = "Adds a quadratic bezier curve.";
		setup.category = { "Drawlist", "Widgets" };
		setup.returnType = mvPyDataType::UUID;

		mvPythonParser parser = FinalizeParser(setup, args);

		parsers->insert({ s_command, parser });
	}

	mvDrawBezierQuadratic::mvDrawBezierQuadratic(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
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
		if (ImPlot::GetCurrentContext()->CurrentPlot)
			drawlist->AddBezierQuadratic(ImPlot::PlotToPixels(_p1), 
				ImPlot::PlotToPixels(_p2), ImPlot::PlotToPixels(_p3), _color, ImPlot::GetCurrentContext()->Mx * _thickness, _segments);
		else
		{
			mvVec2 start = { x, y };
			drawlist->AddBezierQuadratic(_p1 + start, _p2 + start, _p3 + start, _color, _thickness, _segments);
		}
	}

	void mvDrawBezierQuadratic::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!VerifyRequiredArguments(mvApp::GetApp()->getParsers()[s_command], dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				_p1 = ToVec2(item);
				break;

			case 1:
				_p2 = ToVec2(item);
				break;

			case 2:
				_p3 = ToVec2(item);
				break;

			default:
				break;
			}
		}
	}

	void mvDrawBezierQuadratic::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "p1")) _p1 = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "p2")) _p2 = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "p3")) _p3 = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) _thickness = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "segments")) _segments = ToInt(item);
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

}