#include "mvDrawArrow.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvDrawArrow::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Draws an arrow on a drawing.", { "Drawlist", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::FloatList>("p1", mvArgType::REQUIRED_ARG, "...", "Arrow tip.");
		parser.addArg<mvPyDataType::FloatList>("p2", mvArgType::REQUIRED_ARG, "...", "Arrow tail.");

		parser.addArg<mvPyDataType::IntList>("color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)");

		parser.addArg<mvPyDataType::Float>("thickness", mvArgType::KEYWORD_ARG, "1.0");
		parser.addArg<mvPyDataType::Integer>("size", mvArgType::KEYWORD_ARG, "4");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvDrawArrow::mvDrawArrow(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
		updatePoints();
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

		_points.clear();
		_points.push_back({ xsi, ysi });
		_points.push_back({ (float)(x1 - 0.5 * _size * sin(angle)), (float)(y1 + 0.5 * _size * cos(angle)) });
		_points.push_back({ (float)(x1 + 0.5 * _size * cos((M_PI / 2.0) - angle)), (float)(y1 - 0.5 * _size * sin((M_PI / 2.0) - angle)) });

	}

	bool mvDrawArrow::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvStagingContainer) return true;
		if (type == mvAppItemType::mvDrawlist) return true;
		if (type == mvAppItemType::mvDrawLayer) return true;
		if (type == mvAppItemType::mvWindowAppItem) return true;
		if (type == mvAppItemType::mvPlot) return true;
		if (type == mvAppItemType::mvViewportDrawlist) return true;

		mvThrowPythonError(mvErrorCode::mvIncompatibleParent, s_command,
			"Incompatible parent. Acceptable parents include: staging container, drawlist, layer, window, plot, viewport drawlist.", this);
		MV_ITEM_REGISTRY_ERROR("Drawing item parent must be a drawing.");
		assert(false);
		return false;
	}

	void mvDrawArrow::draw(ImDrawList* drawlist, float x, float y)
	{
		
		if (ImPlot::GetCurrentContext()->CurrentPlot)
		{
			drawlist->AddLine(ImPlot::PlotToPixels(_p1), ImPlot::PlotToPixels(_p2), _color, ImPlot::GetCurrentContext()->Mx*_thickness);
			drawlist->AddTriangle(ImPlot::PlotToPixels(_points[0]), ImPlot::PlotToPixels(_points[1]), ImPlot::PlotToPixels(_points[2]), _color, ImPlot::GetCurrentContext()->Mx*_thickness);
			drawlist->AddTriangleFilled(ImPlot::PlotToPixels(_points[0]), ImPlot::PlotToPixels(_points[1]), ImPlot::PlotToPixels(_points[2]), _color);
		}
		else
		{
			mvVec2 start = { x, y };
			drawlist->AddLine(_p1 + start, _p2 + start, _color, _thickness);
			drawlist->AddTriangle(_points[0] + start, _points[1] + start, _points[2] + start, _color, _thickness);
			drawlist->AddTriangleFilled(_points[0] + start, _points[1] + start, _points[2] + start, _color);
		}
	}

	void mvDrawArrow::handleSpecificRequiredArgs(PyObject* dict)
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

			default:
				break;
			}
		}
	}

	void mvDrawArrow::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;


		if (PyObject* item = PyDict_GetItemString(dict, "p1")) _p1 = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "p2")) _p2 = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) _thickness = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "size")) _size = ToFloat(item);

		updatePoints();

	}

	void mvDrawArrow::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		PyDict_SetItemString(dict, "p1", ToPyPair(_p1.x, _p1.y));
		PyDict_SetItemString(dict, "p2", ToPyPair(_p2.x, _p2.y));
		PyDict_SetItemString(dict, "color", ToPyColor(_color));
		PyDict_SetItemString(dict, "thickness", ToPyFloat(_thickness));
		PyDict_SetItemString(dict, "size", ToPyFloat(_size));
	}

}