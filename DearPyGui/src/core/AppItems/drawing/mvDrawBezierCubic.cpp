#include "mvDrawBezierCubic.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvDrawBezierCubic::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Draws a cubic bezier curve on a drawing.", { "Drawlist", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::FloatList>("p1", mvArgType::REQUIRED_ARG, "...", "First point in curve.");
		parser.addArg<mvPyDataType::FloatList>("p2", mvArgType::REQUIRED_ARG, "...", "Second point in curve.");
		parser.addArg<mvPyDataType::FloatList>("p3", mvArgType::REQUIRED_ARG, "...", "Third point in curve.");
		parser.addArg<mvPyDataType::FloatList>("p4", mvArgType::REQUIRED_ARG, "...", "Fourth point in curve.");

		parser.addArg<mvPyDataType::IntList>("color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)");

		parser.addArg<mvPyDataType::Float>("thickness", mvArgType::KEYWORD_ARG, "1.0");

		parser.addArg<mvPyDataType::Integer>("segments", mvArgType::KEYWORD_ARG, "0", "Number of segments to approximate bezier curve.");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvDrawBezierCubic::mvDrawBezierCubic(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

	void mvDrawBezierCubic::draw(ImDrawList* drawlist, float x, float y)
	{
		if (ImPlot::GetCurrentContext()->CurrentPlot)
			drawlist->AddBezierCubic(ImPlot::PlotToPixels(_p1), ImPlot::PlotToPixels(_p2), ImPlot::PlotToPixels(_p3),
				ImPlot::PlotToPixels(_p4), _color, ImPlot::GetCurrentContext()->Mx*_thickness, _segments);
		else
		{
			mvVec2 start = { x, y };
			drawlist->AddBezierCubic(_p1 + start, _p2 + start, _p3 + start, _p4 + start, _color, _thickness, _segments);
		}
	}

	void mvDrawBezierCubic::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyRequiredArguments(dict))
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

			case 3:
				_p4 = ToVec2(item);
				break;

			default:
				break;
			}
		}
	}

	void mvDrawBezierCubic::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "p1")) _p1 = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "p2")) _p2 = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "p3")) _p3 = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "p4")) _p4 = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) _thickness = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "segments")) _segments = ToInt(item);
	}

	void mvDrawBezierCubic::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "p1", ToPyPair(_p1.x, _p1.y));
		PyDict_SetItemString(dict, "p2", ToPyPair(_p2.x, _p2.y));
		PyDict_SetItemString(dict, "p3", ToPyPair(_p3.x, _p3.y));
		PyDict_SetItemString(dict, "p4", ToPyPair(_p4.x, _p4.y));
		PyDict_SetItemString(dict, "color", ToPyColor(_color));
		PyDict_SetItemString(dict, "thickness", ToPyFloat(_thickness));
		PyDict_SetItemString(dict, "segments", ToPyInt(_segments));
	}

}