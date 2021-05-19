#include <utility>
#include "mvHeatSeries.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImPlotThemeScope.h"

namespace Marvel {

	void mvHeatSeries::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String, "Undocumented function", { "Plotting", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_LABEL |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::DoubleList>("x");
		parser.addArg<mvPyDataType::Integer>("rows");
		parser.addArg<mvPyDataType::Integer>("cols");


		parser.addArg<mvPyDataType::Double>("scale_min", mvArgType::KEYWORD_ARG, "0.0");
		parser.addArg<mvPyDataType::Double>("scale_max", mvArgType::KEYWORD_ARG, "1.0");

		parser.addArg<mvPyDataType::DoubleList>("bounds_min", mvArgType::KEYWORD_ARG, "(0.0, 0.0)");
		parser.addArg<mvPyDataType::DoubleList>("bounds_max", mvArgType::KEYWORD_ARG, "(1.0, 1.0)");

		parser.addArg<mvPyDataType::String>("format", mvArgType::KEYWORD_ARG, "'%0.1f'");

		parser.addArg<mvPyDataType::Bool>("contribute_to_bounds", mvArgType::KEYWORD_ARG, "True");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvHeatSeries::mvHeatSeries(const std::string& name)
		: mvSeriesBase(name)
	{
	}

	void mvHeatSeries::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImPlotThemeScope scope(this);

		static const std::vector<double>* xptr;

		xptr = &(*m_value.get())[0];


		ImPlot::PlotHeatmap(m_label.c_str(), xptr->data(), m_rows, m_cols, m_scale_min, m_scale_max,
			m_format.c_str(), { m_bounds_min.x, m_bounds_min.y }, { m_bounds_max.x, m_bounds_max.y });

	}

	void mvHeatSeries::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyRequiredArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				(*m_value)[0] = ToDoubleVect(item);
				break;

			case 1:
				m_rows = ToInt(item);
				break;

			case 2:
				m_cols = ToInt(item);
				break;


			default:
				break;
			}
		}

		resetMaxMins();
		calculateMaxMins();
	}

	void mvHeatSeries::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "rows")) m_rows = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "cols")) m_cols = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "bounds_min")) m_bounds_min = ToPoint(item);
		if (PyObject* item = PyDict_GetItemString(dict, "bounds_max")) m_bounds_max = ToPoint(item);
		if (PyObject* item = PyDict_GetItemString(dict, "scale_min")) m_scale_min = ToDouble(item);
		if (PyObject* item = PyDict_GetItemString(dict, "scale_max")) m_scale_max = ToDouble(item);
		if (PyObject* item = PyDict_GetItemString(dict, "contribute_to_bounds")) m_contributeToBounds = ToBool(item);

		bool valueChanged = false;
		if (PyObject* item = PyDict_GetItemString(dict, "x")) { valueChanged = true; (*m_value)[0] = ToDoubleVect(item); }

		if (valueChanged)
		{
			resetMaxMins();
			calculateMaxMins();
		}

	}

	void mvHeatSeries::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "format", ToPyString(m_format));
		PyDict_SetItemString(dict, "rows", ToPyInt(m_rows));
		PyDict_SetItemString(dict, "cols", ToPyInt(m_cols));
		PyDict_SetItemString(dict, "bounds_min", ToPyPair(m_bounds_min.x, m_bounds_min.y));
		PyDict_SetItemString(dict, "bounds_max", ToPyPair(m_bounds_max.x, m_bounds_max.y));
		PyDict_SetItemString(dict, "scale_min", ToPyDouble(m_scale_min));
		PyDict_SetItemString(dict, "scale_max", ToPyDouble(m_scale_max));
		PyDict_SetItemString(dict, "contribute_to_bounds", ToPyBool(m_contributeToBounds));
	}

}