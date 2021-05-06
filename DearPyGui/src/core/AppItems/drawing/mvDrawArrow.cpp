#include "mvDrawArrow.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvDrawArrow::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String, "Undocumented function", { "Drawlist", "Widgets" });
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("source");
		parser.removeArg("width");
		parser.removeArg("height");
		parser.removeArg("label");
		parser.removeArg("callback");
		parser.removeArg("callback_data");
		parser.removeArg("enabled");

		parser.addArg<mvPyDataType::FloatList>("p1");
		parser.addArg<mvPyDataType::FloatList>("p2");

		parser.addArg<mvPyDataType::IntList>("color", mvArgType::KEYWORD_ARG, "(255, 255, 255, 255)");

		parser.addArg<mvPyDataType::Float>("thickness", mvArgType::KEYWORD_ARG, "1.0");
		parser.addArg<mvPyDataType::Integer>("size", mvArgType::KEYWORD_ARG, "4");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvDrawArrow::mvDrawArrow(const std::string& name)
		:
		mvAppItem(name)
	{
		updatePoints();
	}

	void mvDrawArrow::updatePoints()
	{
		float xsi = m_p1.x;
		float xfi = m_p2.x;
		float ysi = m_p1.y;
		float yfi = m_p2.y;

		// length of arrow head
		double xoffset = m_size;
		double yoffset = m_size;

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

		m_points.clear();
		m_points.push_back({ xsi, ysi });
		m_points.push_back({ (float)(x1 - 0.5 * m_size * sin(angle)), (float)(y1 + 0.5 * m_size * cos(angle)) });
		m_points.push_back({ (float)(x1 + 0.5 * m_size * cos((M_PI / 2.0) - angle)), (float)(y1 - 0.5 * m_size * sin((M_PI / 2.0) - angle)) });

	}

	bool mvDrawArrow::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvStagingContainer) return true;
		if (type == mvAppItemType::mvDrawlist) return true;
		if (type == mvAppItemType::mvDrawLayer) return true;
		if (type == mvAppItemType::mvWindowAppItem) return true;
		if (type == mvAppItemType::mvPlot) return true;
		if (type == mvAppItemType::mvViewportDrawlist) return true;

		mvThrowPythonError(1000, "Drawing item parent must be a drawing.");
		MV_ITEM_REGISTRY_ERROR("Drawing item parent must be a drawing.");
		assert(false);
		return false;
	}

	void mvDrawArrow::draw(ImDrawList* drawlist, float x, float y)
	{
		mvVec2 start = { x, y };
		drawlist->AddLine(m_p1 + start, m_p2 + start, m_color, m_thickness);
		drawlist->AddTriangle(m_points[0] + start, m_points[1] + start, m_points[2] + start, m_color, m_thickness);
		drawlist->AddTriangleFilled(m_points[0] + start, m_points[1] + start, m_points[2] + start, m_color);
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
				m_p1 = ToVec2(item);
				break;

			case 1:
				m_p2 = ToVec2(item);
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


		if (PyObject* item = PyDict_GetItemString(dict, "p1")) m_p1 = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "p2")) m_p2 = ToVec2(item);
		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "thickness")) m_thickness = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "size")) m_size = ToFloat(item);

		updatePoints();

	}

	void mvDrawArrow::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		PyDict_SetItemString(dict, "p1", ToPyPair(m_p1.x, m_p1.y));
		PyDict_SetItemString(dict, "p2", ToPyPair(m_p2.x, m_p2.y));
		PyDict_SetItemString(dict, "color", ToPyColor(m_color));
		PyDict_SetItemString(dict, "thickness", ToPyFloat(m_thickness));
		PyDict_SetItemString(dict, "size", ToPyFloat(m_size));
	}

}