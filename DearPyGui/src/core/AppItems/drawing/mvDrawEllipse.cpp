#include "mvDrawEllipse.h"
#include "mvDrawPolygon.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include <math.h>

namespace Marvel {

	void mvDrawEllipse::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String, "Undocumented function", { "Drawlist", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::FloatList>("pmin");
		parser.addArg<mvPyDataType::FloatList>("pmax");

		parser.addArg<mvPyDataType::IntList>("color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)");
		parser.addArg<mvPyDataType::IntList>("fill", mvArgType::KEYWORD_ARG, "(0, 0, 0, -255)");

		parser.addArg<mvPyDataType::Float>("thickness", mvArgType::KEYWORD_ARG, "1.0");

		parser.addArg<mvPyDataType::Integer>("segments", mvArgType::KEYWORD_ARG, "3");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvDrawEllipse::mvDrawEllipse(const std::string& name)
		:
		mvAppItem(name)
	{
	}

	bool mvDrawEllipse::isParentCompatible(mvAppItemType type)
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

	void mvDrawEllipse::draw(ImDrawList* drawlist, float x, float y)
	{
		if (m_dirty) {
			if (m_segments < 3) { m_segments = 3; }
			const float  width = m_pmax.x - m_pmin.x;
			const float  height = m_pmax.y - m_pmin.y;
			const float  cx = width / 2.0f + m_pmin.x;
			const float  cy = height / 2.0f + m_pmin.y;
			const float a_max = ((float)M_PI * 2.0f) * ((float)m_segments / (float)m_segments);
			std::vector<mvVec2> points;
			points.reserve(m_segments + 1);
			for (int i = 0; i <= m_segments; i++)
			{
				const float a = ((float)i / (float)m_segments) * a_max;
				points.push_back(mvVec2{ cx  + cosf(a) * width, cy + sinf(a) * height });
			}
			m_points = points;
			m_dirty = false;
		}

		std::vector<mvVec2> points = m_points;
		for (auto& point : points) {
			point.x += x;
			point.y += y;
		}

		if(m_fill.r > 0.0f)
			drawlist->AddConvexPolyFilled((const ImVec2*)const_cast<const mvVec2*>(points.data()), (int)points.size(), m_fill);
		else
			drawlist->AddPolyline((const ImVec2*)const_cast<const mvVec2*>(points.data()), (int)points.size(), m_color, false, m_thickness);
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
				m_pmin = ToVec2(item);
				m_dirty = true;
				break;

			case 1:
				m_pmax = ToVec2(item);
				m_dirty = true;
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


		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "fill")) m_fill = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) m_thickness = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "m_pmax")) m_pmax = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "m_pmin")) m_pmin = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "segments")) m_segments = ToInt(item);

	}

	void mvDrawEllipse::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "color", ToPyColor(m_color));
		PyDict_SetItemString(dict, "fill", ToPyColor(m_fill));
		PyDict_SetItemString(dict, "thickness", ToPyFloat(m_thickness));
		PyDict_SetItemString(dict, "pmax", ToPyPair(m_pmax.x, m_pmax.y));
		PyDict_SetItemString(dict, "pmin", ToPyPair(m_pmin.x, m_pmin.y));
		PyDict_SetItemString(dict, "segments", ToPyInt(m_segments));
	}

}