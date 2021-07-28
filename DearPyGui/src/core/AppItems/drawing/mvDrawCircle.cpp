#include "mvDrawCircle.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvDrawCircle::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Draws a circle on a drawing.", { "Drawlist", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::FloatList>("center");

		parser.addArg<mvPyDataType::Float>("radius");

		parser.addArg<mvPyDataType::IntList>("color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)");
		parser.addArg<mvPyDataType::IntList>("fill", mvArgType::KEYWORD_ARG, "(0, 0, 0, -255)");

		parser.addArg<mvPyDataType::Float>("thickness", mvArgType::KEYWORD_ARG, "1.0");

		parser.addArg<mvPyDataType::Integer>("segments", mvArgType::KEYWORD_ARG, "0", "Number of segments to approximate circle.");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvDrawCircle::mvDrawCircle(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

	bool mvDrawCircle::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvStagingContainer) return true;
		if (type == mvAppItemType::mvDrawlist) return true;
		if (type == mvAppItemType::mvWindowAppItem) return true;
		if (type == mvAppItemType::mvPlot) return true;
		if (type == mvAppItemType::mvDrawLayer) return true;
		if (type == mvAppItemType::mvViewportDrawlist) return true;

		mvThrowPythonError(mvErrorCode::mvIncompatibleParent, s_command,
			"Incompatible parent. Acceptable parents include: staging container, drawlist, layer, window, plot, viewport drawlist.", this);

		MV_ITEM_REGISTRY_ERROR("Drawing item parent must be a drawing.");
		assert(false);
		return false;
	}

	void mvDrawCircle::draw(ImDrawList* drawlist, float x, float y)
	{
		if (ImPlot::GetCurrentContext()->CurrentPlot)
		{
			drawlist->AddCircle(ImPlot::PlotToPixels(_center), ImPlot::GetCurrentContext()->Mx * _radius, _color, 
				ImPlot::GetCurrentContext()->Mx * _segments, _thickness);
			if (_fill.r < 0.0f)
				return;
			drawlist->AddCircleFilled(ImPlot::PlotToPixels(_center), ImPlot::GetCurrentContext()->Mx * _radius, _fill, _segments);
		}
		else
		{
		mvVec2 start = { x, y };
		drawlist->AddCircle(_center + start, _radius, _color, _segments, _thickness);
		if (_fill.r < 0.0f)
			return;
		drawlist->AddCircleFilled(_center + start, _radius, _fill, _segments);
		}
	}

	void mvDrawCircle::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyRequiredArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				_center = ToVec2(item);
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


		if (PyObject* item = PyDict_GetItemString(dict, "center")) _center = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "fill")) _fill = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) _thickness = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "radius")) _radius = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "segments")) _segments = ToInt(item);

	}

	void mvDrawCircle::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "center", ToPyPair(_center.x, _center.y));
		PyDict_SetItemString(dict, "color", ToPyColor(_color));
		PyDict_SetItemString(dict, "fill", ToPyColor(_fill));
		PyDict_SetItemString(dict, "thickness", ToPyFloat(_thickness));
		PyDict_SetItemString(dict, "radius", ToPyFloat(_radius));
		PyDict_SetItemString(dict, "segments", ToPyInt(_segments));
	}

}