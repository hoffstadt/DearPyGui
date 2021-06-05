#include <utility>
#include "mvErrorSeries.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImPlotThemeScope.h"

namespace Marvel {

	void mvErrorSeries::InsertParser(std::map<std::string, mvPythonParser>* parsers)
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
		parser.addArg<mvPyDataType::DoubleList>("y");
		parser.addArg<mvPyDataType::DoubleList>("negative");
		parser.addArg<mvPyDataType::DoubleList>("positive");

		parser.addArg<mvPyDataType::Bool>("contribute_to_bounds", mvArgType::KEYWORD_ARG, "True");
		parser.addArg<mvPyDataType::Bool>("horizontal", mvArgType::KEYWORD_ARG, "False");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvErrorSeries::mvErrorSeries(const std::string& name)
		: mvSeriesBase(name)
	{
		m_contributeToBounds = true;
	}

	void mvErrorSeries::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImPlotThemeScope scope(this);

		static const std::vector<double>* xptr;
		static const std::vector<double>* yptr;
		static const std::vector<double>* zptr;
		static const std::vector<double>* wptr;

		xptr = &(*m_value.get())[0];
		yptr = &(*m_value.get())[1];
		zptr = &(*m_value.get())[2];
		wptr = &(*m_value.get())[3];

		if (m_horizontal)
			ImPlot::PlotErrorBarsH(m_label.c_str(), xptr->data(), yptr->data(), zptr->data(), wptr->data(), (int)xptr->size());
		else
			ImPlot::PlotErrorBars(m_label.c_str(), xptr->data(), yptr->data(), zptr->data(), wptr->data(), (int)xptr->size());

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

	void mvErrorSeries::handleSpecificRequiredArgs(PyObject* dict)
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

			case 2:
				(*m_value)[2] = ToDoubleVect(item);
				break;

			case 3:
				(*m_value)[3] = ToDoubleVect(item);
				break;

			default:
				break;
			}
		}

		resetMaxMins();
		calculateMaxMins();
	}

	void mvErrorSeries::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "contribute_to_bounds")) m_contributeToBounds = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "horizontal")) m_horizontal= ToBool(item);

		bool valueChanged = false;
		if (PyObject* item = PyDict_GetItemString(dict, "x")) { valueChanged = true; (*m_value)[0] = ToDoubleVect(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "y")) { valueChanged = true; (*m_value)[1] = ToDoubleVect(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "negative")) { valueChanged = true; (*m_value)[2] = ToDoubleVect(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "positive")) { valueChanged = true; (*m_value)[3] = ToDoubleVect(item); }

		if (valueChanged)
		{
			resetMaxMins();
			calculateMaxMins();
		}

	}

	void mvErrorSeries::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "contribute_to_bounds", ToPyBool(m_contributeToBounds));
		PyDict_SetItemString(dict, "horizontal", ToPyBool(m_horizontal));
	}

}