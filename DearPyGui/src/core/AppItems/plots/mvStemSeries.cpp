#include <utility>
#include "mvStemSeries.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImPlotThemeScope.h"

namespace Marvel {

	void mvStemSeries::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::String, "Undocumented function", { "Plotting", "Widgets" });
		mvAppItem::AddCommonArgs(parser);
		parser.removeArg("width");
		parser.removeArg("height");
		parser.removeArg("callback");
		parser.removeArg("callback_data");
		parser.removeArg("enabled");

		parser.addArg<mvPyDataType::DoubleList>("x");
		parser.addArg<mvPyDataType::DoubleList>("y");

		parser.addArg<mvPyDataType::Integer>("axis", mvArgType::KEYWORD_ARG, "0");

		parser.addArg<mvPyDataType::Bool>("contribute_to_bounds", mvArgType::KEYWORD_ARG, "True");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvStemSeries::mvStemSeries(const std::string& name)
		: mvSeriesBase(name)
	{
		m_contributeToBounds = true;
	}

	void mvStemSeries::draw(ImDrawList* drawlist, float x, float y)
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

		static const std::vector<double>* xptr;
		static const std::vector<double>* yptr;

		xptr = &(*m_value.get())[0];
		yptr = &(*m_value.get())[1];

		ImPlot::PlotStems(m_label.c_str(), xptr->data(), yptr->data(), (int)xptr->size());

	}

	void mvStemSeries::handleSpecificRequiredArgs(PyObject* dict)
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
				(*m_value)[1] = ToDoubleVect(item);
				break;

			default:
				break;
			}
		}

		resetMaxMins();
		calculateMaxMins();
	}

	void mvStemSeries::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "axis")) m_axis = (ImPlotYAxis_)ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "contribute_to_bounds")) m_contributeToBounds = ToBool(item);

		bool valueChanged = false;
		if (PyObject* item = PyDict_GetItemString(dict, "x")) { valueChanged = true; (*m_value)[0] = ToDoubleVect(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "y")) { valueChanged = true; (*m_value)[1] = ToDoubleVect(item); }

		if (valueChanged)
		{
			resetMaxMins();
			calculateMaxMins();
		}
	}

	void mvStemSeries::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "axis", ToPyInt(m_axis));
		PyDict_SetItemString(dict, "contribute_to_bounds", ToPyBool(m_contributeToBounds));
	}

}