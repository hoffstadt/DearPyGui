#include <utility>
#include "mvHistogramSeries.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImPlotThemeScope.h"

namespace Marvel {

	void mvHistogramSeries::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Plotting", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_LABEL |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::DoubleList>("x");


		parser.addArg<mvPyDataType::Integer>("bins", mvArgType::KEYWORD_ARG, "-1");
		parser.addArg<mvPyDataType::Float>("bar_scale", mvArgType::KEYWORD_ARG, "1.0");
		parser.addArg<mvPyDataType::Double>("min_range", mvArgType::KEYWORD_ARG, "0.0");
		parser.addArg<mvPyDataType::Double>("max_range", mvArgType::KEYWORD_ARG, "1.0");
		parser.addArg<mvPyDataType::Bool>("cumlative", mvArgType::KEYWORD_ARG, "False");
		parser.addArg<mvPyDataType::Bool>("density", mvArgType::KEYWORD_ARG, "False");
		parser.addArg<mvPyDataType::Bool>("outliers", mvArgType::KEYWORD_ARG, "True");

		parser.addArg<mvPyDataType::Bool>("contribute_to_bounds", mvArgType::KEYWORD_ARG, "True");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvHistogramSeries::mvHistogramSeries(mvUUID uuid)
		: mvSeriesBase(uuid)
	{
		m_contributeToBounds = true;
	}

	void mvHistogramSeries::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(m_uuid);
		mvImPlotThemeScope scope(this);

		static const std::vector<double>* xptr;

		xptr = &(*m_value.get())[0];

		ImPlot::PlotHistogram(m_label.c_str(), xptr->data(), (int)xptr->size(), m_bins,
			m_cumlative, m_density, ImPlotRange(m_min, m_max), m_outliers, (double)m_barScale);

		// Begin a popup for a legend entry.
		if (ImPlot::BeginLegendPopup(m_label.c_str(), 1))
		{
			for (auto& childset : m_children)
			{
				for (auto& item : childset)
				{
					// skip item if it's not shown
					if (!item->m_show)
						continue;
					item->draw(drawlist, ImPlot::GetPlotPos().x, ImPlot::GetPlotPos().y);
					item->getState().update();
				}
			}
			ImPlot::EndLegendPopup();
		}

	}

	void mvHistogramSeries::handleSpecificRequiredArgs(PyObject* dict)
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

			default:
				break;
			}
		}

		resetMaxMins();
		calculateMaxMins();
	}

	void mvHistogramSeries::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "contribute_to_bounds")) m_contributeToBounds = ToBool(item);

		bool valueChanged = false;
		if (PyObject* item = PyDict_GetItemString(dict, "x")) { valueChanged = true; (*m_value)[0] = ToDoubleVect(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "bins")) { valueChanged = true; m_bins = ToInt(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "bar_scale")) { valueChanged = true; m_barScale = ToFloat(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "min_range")) { valueChanged = true; m_min = ToDouble(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "max_range")) { valueChanged = true; m_max = ToDouble(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "cumlative")) { valueChanged = true; m_cumlative = ToBool(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "density")) { valueChanged = true; m_density = ToBool(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "outliers")) { valueChanged = true; m_outliers = ToBool(item); }

		if (valueChanged)
		{
			resetMaxMins();
			calculateMaxMins();
		}

	}

	void mvHistogramSeries::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "contribute_to_bounds", ToPyBool(m_contributeToBounds));
	}

}