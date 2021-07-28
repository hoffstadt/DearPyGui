#include "mvDrawTriangle.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvDrawTriangle::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Draws a triangle on a drawing.", { "Drawlist", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::FloatList>("p1");
		parser.addArg<mvPyDataType::FloatList>("p2");
		parser.addArg<mvPyDataType::FloatList>("p3");

		parser.addArg<mvPyDataType::IntList>("color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)");
		parser.addArg<mvPyDataType::IntList>("fill", mvArgType::KEYWORD_ARG, "(0, 0, 0, -255)");

		parser.addArg<mvPyDataType::Float>("thickness", mvArgType::KEYWORD_ARG, "1.0");

		parser.finalize();

		parsers->insert({ s_command, parser });

	}

	mvDrawTriangle::mvDrawTriangle(mvUUID uuid)
		:
		mvAppItem(uuid)
	{
	}

	bool mvDrawTriangle::isParentCompatible(mvAppItemType type)
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

	void mvDrawTriangle::draw(ImDrawList* drawlist, float x, float y)
	{
		if (ImPlot::GetCurrentContext()->CurrentPlot)
		{
			drawlist->AddTriangle(ImPlot::PlotToPixels(_p1), ImPlot::PlotToPixels(_p2), ImPlot::PlotToPixels(_p3),
				_color, ImPlot::GetCurrentContext()->Mx * _thickness);
			if (_fill.r < 0.0f)
				return;
			drawlist->AddTriangleFilled(ImPlot::PlotToPixels(_p1), ImPlot::PlotToPixels(_p2), ImPlot::PlotToPixels(_p3), 
				_fill);
		}
		else
		{
			mvVec2 start = { x, y };
			drawlist->AddTriangle(_p1 + start, _p2 + start, _p3 + start, _color, _thickness);
			if (_fill.r < 0.0f)
				return;
			drawlist->AddTriangleFilled(_p1 + start, _p2 + start, _p3 + start, _fill);
		}
	}

	void mvDrawTriangle::handleSpecificRequiredArgs(PyObject* dict)
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

			default:
				break;
			}
		}
	}

	void mvDrawTriangle::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "p1")) _p1 = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "p2")) _p2 = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "p3")) _p3 = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) _color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "fill")) _fill = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) _thickness = ToFloat(item);

	}

	void mvDrawTriangle::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "p1", ToPyPair(_p1.x, _p1.y));
		PyDict_SetItemString(dict, "p2", ToPyPair(_p2.x, _p2.y));
		PyDict_SetItemString(dict, "p3", ToPyPair(_p3.x, _p3.y));
		PyDict_SetItemString(dict, "color", ToPyColor(_color));
		PyDict_SetItemString(dict, "thickness", ToPyColor(_fill));
		PyDict_SetItemString(dict, "thickness", ToPyFloat(_thickness));
	}

}