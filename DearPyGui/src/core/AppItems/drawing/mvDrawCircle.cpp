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
			drawlist->AddCircle(ImPlot::PlotToPixels(m_center), m_radius, m_color, m_segments, m_thickness);
			if (m_fill.r < 0.0f)
				return;
			drawlist->AddCircleFilled(ImPlot::PlotToPixels(m_center), m_radius, m_fill, m_segments);
		}
		else
		{
		mvVec2 start = { x, y };
		drawlist->AddCircle(m_center + start, m_radius, m_color, m_segments, m_thickness);
		if (m_fill.r < 0.0f)
			return;
		drawlist->AddCircleFilled(m_center + start, m_radius, m_fill, m_segments);
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
				m_center = ToVec2(item);
				break;

			case 1:
				m_radius = ToFloat(item);
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


		if (PyObject* item = PyDict_GetItemString(dict, "center")) m_center = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "fill")) m_fill = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) m_thickness = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "radius")) m_radius = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "segments")) m_segments = ToInt(item);

	}

	void mvDrawCircle::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "center", ToPyPair(m_center.x, m_center.y));
		PyDict_SetItemString(dict, "color", ToPyColor(m_color));
		PyDict_SetItemString(dict, "fill", ToPyColor(m_fill));
		PyDict_SetItemString(dict, "thickness", ToPyFloat(m_thickness));
		PyDict_SetItemString(dict, "radius", ToPyFloat(m_radius));
		PyDict_SetItemString(dict, "segments", ToPyInt(m_segments));
	}

}