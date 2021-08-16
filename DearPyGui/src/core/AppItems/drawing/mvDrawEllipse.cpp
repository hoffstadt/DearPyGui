#include "mvDrawEllipse.h"
#include "mvDrawPolygon.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include <math.h>

namespace Marvel {

	void mvDrawEllipse::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Draws an ellipse on a drawing.", { "Drawlist", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::FloatList>("pmin", mvArgType::REQUIRED_ARG, "...", "Min point of bounding rectangle.");
		parser.addArg<mvPyDataType::FloatList>("pmax", mvArgType::REQUIRED_ARG, "...", "Max point of bounding rectangle.");

		parser.addArg<mvPyDataType::IntList>("color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)");
		parser.addArg<mvPyDataType::IntList>("fill", mvArgType::KEYWORD_ARG, "(0, 0, 0, -255)");

		parser.addArg<mvPyDataType::Float>("thickness", mvArgType::KEYWORD_ARG, "1.0");

		parser.addArg<mvPyDataType::Integer>("segments", mvArgType::KEYWORD_ARG, "32", "Number of segments to approximate bezier curve.");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvDrawEllipse::mvDrawEllipse(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

	void mvDrawEllipse::draw(ImDrawList* drawlist, float x, float y)
	{
		if (_dirty) {
			if (_segments < 3) { _segments = 3; }
			const float  width = _pmax.x - _pmin.x;
			const float  height = _pmax.y - _pmin.y;
			const float  cx = width / 2.0f + _pmin.x;
			const float  cy = height / 2.0f + _pmin.y;
			const float radian_inc = ((float)M_PI * 2.0f) / (float)_segments;
			std::vector<mvVec2> points;
			points.reserve(_segments + 1);
			for (int i = 0; i <= _segments; i++)
			{
				points.push_back(mvVec2{ cx  + cosf(i*radian_inc) * width, cy + sinf(i * radian_inc) * height });
			}
			_points = points;
			_dirty = false;
		}

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
			{
				point.x += x;
				point.y += y;
			}
		}

		drawlist->AddPolyline((const ImVec2*)const_cast<const mvVec2*>(points.data()), (int)points.size(), 
			_color, false, ImPlot::GetCurrentContext()->Mx * _thickness);
		if (_fill.r < 0.0f)
			return;
		drawlist->AddConvexPolyFilled((const ImVec2*)const_cast<const mvVec2*>(points.data()), (int)points.size(), _fill);
	}

	void mvDrawEllipse::handleSpecificRequiredArgs(PyObject* dict)
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
				_dirty = true;
				break;

			case 1:
				_pmax = ToVec2(item);
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
		if (PyObject* item = PyDict_GetItemString(dict, "_pmax")) _pmax = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "_pmin")) _pmin = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "segments")) _segments = ToInt(item);

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