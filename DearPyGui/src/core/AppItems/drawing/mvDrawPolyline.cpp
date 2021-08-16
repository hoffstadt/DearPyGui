#include "mvDrawPolyline.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvDrawPolyline::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Draws connected lines on a drawing from points.", { "Drawlist", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::ListFloatList>("points");

		parser.addArg<mvPyDataType::Bool>("closed", mvArgType::KEYWORD_ARG, "False", "Will close the polyline by returning to the first point.");

		parser.addArg<mvPyDataType::IntList>("color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)");

		parser.addArg<mvPyDataType::Float>("thickness", mvArgType::KEYWORD_ARG, "1.0");


		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvDrawPolyline::mvDrawPolyline(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

	void mvDrawPolyline::draw(ImDrawList* drawlist, float x, float y)
	{

		mvVec2 start = { x, y };

		std::vector<mvVec2> points = _points;

		if (ImPlot::GetCurrentContext()->CurrentPlot)
		{
			for (auto& point : points)
			{
				ImVec2 impoint = ImPlot::PlotToPixels(point);
				point.x = impoint.x;
				point.y = impoint.y;
			}
		}
		else
		{
			for (auto& point : points)
				point = point + start;
		}

		drawlist->AddPolyline((const ImVec2*)const_cast<const mvVec2*>(points.data()), (int)_points.size(), _color, 
			_closed, ImPlot::GetCurrentContext()->Mx * _thickness);
	}

	void mvDrawPolyline::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyRequiredArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				_points = ToVectVec2(item);
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

		if (PyObject* item = PyDict_GetItemString(dict, "points")) _points = ToVectVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "closed")) _closed = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) _thickness = ToFloat(item);
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