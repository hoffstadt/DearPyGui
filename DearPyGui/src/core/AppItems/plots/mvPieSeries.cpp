#include <utility>
#include "mvPieSeries.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImPlotThemeScope.h"

namespace Marvel {

	void mvPieSeries::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String);
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("width");
		parser.removeArg("height");
		parser.removeArg("callback");
		parser.removeArg("callback_data");
		parser.removeArg("enabled");

		parser.addArg<mvPyDataType::Double>("x");
		parser.addArg<mvPyDataType::Double>("y");
		parser.addArg<mvPyDataType::Double>("radius");
		parser.addArg<mvPyDataType::FloatList>("values");
		parser.addArg<mvPyDataType::StringList>("labels");

		parser.addArg<mvPyDataType::String>("format", mvArgType::KEYWORD_ARG, "'%0.2f'");

		parser.addArg<mvPyDataType::Double>("angle", mvArgType::KEYWORD_ARG, "90.0");

		parser.addArg<mvPyDataType::Integer>("axis", mvArgType::KEYWORD_ARG, "0");

		parser.addArg<mvPyDataType::Bool>("normalize", mvArgType::KEYWORD_ARG, "False");
		parser.addArg<mvPyDataType::Bool>("contribute_to_bounds", mvArgType::KEYWORD_ARG, "True");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvPieSeries::mvPieSeries(const std::string& name)
		: mvSeriesBase(name)
	{
	}

	void mvPieSeries::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImPlotThemeScope scope(this);

		switch (m_axis)
		{
		case ImPlotYAxis_1:
			ImPlot::SetPlotYAxis(ImPlotYAxis_1);
			break;
		case ImPlotYAxis_2:
			ImPlot::SetPlotYAxis(ImPlotYAxis_2);
			break;
		case ImPlotYAxis_3:
			ImPlot::SetPlotYAxis(ImPlotYAxis_3);
			break;
		default:
			break;
		}

		static const std::vector<float>* xptr;

		xptr = &(*m_value.get())[0];

		ImPlot::PlotPieChart(m_clabels.data(), xptr->data(), (int)m_labels.size(),
			m_x, m_y, m_radius, m_normalize, m_format.c_str(), m_angle);

	}

	void mvPieSeries::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyRequiredArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				m_x = ToFloat(item);
				break;

			case 1:
				m_y = ToFloat(item);
				break;

			case 2:
				m_radius = ToFloat(item);
				break;

			case 3:
				(*m_value)[0] = ToFloatVect(item);
				break;

			case 4:
				m_labels = ToStringVect(item);
				m_clabels.clear();
				for (const auto& label : m_labels)
					m_clabels.push_back(label.c_str());
				break;

			default:
				break;
			}
		}

		resetMaxMins();
		calculateMaxMins();
	}

	void mvPieSeries::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "axis")) m_axis = (ImPlotYAxis_)ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "x")) m_x = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "y")) m_y = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "radius")) m_radius = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "angle")) m_angle = ToFloat(item);
		if (PyObject* item = PyDict_GetItemString(dict, "normalize")) m_normalize = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "labels"))
		{
			m_labels = ToStringVect(item);
			m_clabels.clear();
			for (const auto& label : m_labels)
				m_clabels.push_back(label.c_str());
		}

		bool valueChanged = false;
		if (PyObject* item = PyDict_GetItemString(dict, "values")) { valueChanged = true; (*m_value)[0] = ToFloatVect(item); }
		if (valueChanged)
		{
			resetMaxMins();
			calculateMaxMins();
		}


	}

	void mvPieSeries::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	}

}