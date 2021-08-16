#include "mvDrawRect.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvPyObject.h"

namespace Marvel {

	void mvDrawRect::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Draws a rectangle on a drawing.", { "Drawlist", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::FloatList>("pmin", mvArgType::REQUIRED_ARG, "...", "Min point of bounding rectangle.");
		parser.addArg<mvPyDataType::FloatList>("pmax", mvArgType::REQUIRED_ARG, "...", "Max point of bounding rectangle.");

		parser.addArg<mvPyDataType::IntList>("color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)");
		parser.addArg<mvPyDataType::IntList>("color_upper_left", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)", "'multicolor' must be set to 'True'");
		parser.addArg<mvPyDataType::IntList>("color_upper_right", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)", "'multicolor' must be set to 'True'");
		parser.addArg<mvPyDataType::IntList>("color_bottom_right", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)", "'multicolor' must be set to 'True'");
		parser.addArg<mvPyDataType::IntList>("color_bottom_left", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)", "'multicolor' must be set to 'True'");
		parser.addArg<mvPyDataType::IntList>("fill", mvArgType::KEYWORD_ARG, "(0, 0, 0, -255)");
		parser.addArg<mvPyDataType::Bool>("multicolor", mvArgType::KEYWORD_ARG, "False");

		parser.addArg<mvPyDataType::Float>("rounding", mvArgType::KEYWORD_ARG, "0.0", "Number of pixels of the radius that will round the corners of the rectangle. Note: doesn't work with multicolor");
		parser.addArg<mvPyDataType::Float>("thickness", mvArgType::KEYWORD_ARG, "1.0");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvDrawRect::mvDrawRect(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

	void mvDrawRect::draw(ImDrawList* drawlist, float x, float y)
	{
		if (ImPlot::GetCurrentContext()->CurrentPlot)
		{
			drawlist->AddRect(ImPlot::PlotToPixels(_pmin), ImPlot::PlotToPixels(_pmax), _color, 
				ImPlot::GetCurrentContext()->Mx * _rounding, ImDrawCornerFlags_All, ImPlot::GetCurrentContext()->Mx * _thickness);
			if (_multicolor)
			{
				drawlist->AddRectFilledMultiColor(ImPlot::PlotToPixels(_pmin), ImPlot::PlotToPixels(_pmax), _color_bottom_right, _color_bottom_left, _color_upper_left, _color_upper_right);
				return;
			}
			if (_fill.r < 0.0f)
				return;
			drawlist->AddRectFilled(ImPlot::PlotToPixels(_pmin), ImPlot::PlotToPixels(_pmax), _fill, ImPlot::GetCurrentContext()->Mx * _rounding, ImDrawCornerFlags_All);
		}
		else
		{
			mvVec2 start = { x, y };
			drawlist->AddRect(_pmin + start, _pmax + start, _color, _rounding, ImDrawCornerFlags_All, _thickness);
			if (_multicolor)
			{
				drawlist->AddRectFilledMultiColor(_pmin + start, _pmax + start, _color_bottom_right, _color_bottom_left, _color_upper_left, _color_upper_right);
				return;
			}
			if (_fill.r < 0.0f)
				return;
			drawlist->AddRectFilled(_pmin + start, _pmax + start, _fill, _rounding, ImDrawCornerFlags_All);
		}
	}

	void mvDrawRect::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyRequiredArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				_pmin = ToVec2(item);
				break;

			case 1:
				_pmax = ToVec2(item);
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

		if (PyObject* item = PyDict_GetItemString(dict, "pmax")) _pmax = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "pmin")) _pmin = ToVec2(item);
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