#include "mvDrawPolyline.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvDrawPolyline::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String, "Undocumented function", { "Drawlist", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::ListFloatList>("points");

		parser.addArg<mvPyDataType::Bool>("closed", mvArgType::KEYWORD_ARG, "False");

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

	bool mvDrawPolyline::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvStagingContainer) return true;
		if (type == mvAppItemType::mvDrawlist) return true;
		if (type == mvAppItemType::mvWindowAppItem) return true;
		if (type == mvAppItemType::mvPlot) return true;
		if (type == mvAppItemType::mvDrawLayer) return true;
		if (type == mvAppItemType::mvViewportDrawlist) return true;

		mvThrowPythonError(1000, "Drawing item parent must be a drawing.");
		MV_ITEM_REGISTRY_ERROR("Drawing item parent must be a drawing.");
		assert(false);
		return false;
	}

	void mvDrawPolyline::draw(ImDrawList* drawlist, float x, float y)
	{
		mvVec2 start = { x, y };

		std::vector<mvVec2> points = m_points;

		for (auto& point : points)
			point = point + start;

		drawlist->AddPolyline((const ImVec2*)const_cast<const mvVec2*>(points.data()), (int)m_points.size(), m_color, m_closed, m_thickness);
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
				m_points = ToVectVec2(item);
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

		if (PyObject* item = PyDict_GetItemString(dict, "points")) m_points = ToVectVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "closed")) m_closed = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) m_thickness = ToFloat(item);
	}

	void mvDrawPolyline::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "points", ToPyList(m_points));
		PyDict_SetItemString(dict, "closed", ToPyBool(m_closed));
		PyDict_SetItemString(dict, "color", ToPyColor(m_color));
		PyDict_SetItemString(dict, "thickness", ToPyFloat(m_thickness));
	}
}