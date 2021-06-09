#include <utility>
#include "mvShadeSeries.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImPlotThemeScope.h"

namespace Marvel {

	void mvShadeSeries::InsertParser(std::map<std::string, mvPythonParser>* parsers)
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
		parser.addArg<mvPyDataType::DoubleList>("y1");

		parser.addArg<mvPyDataType::DoubleList>("y2", mvArgType::KEYWORD_ARG, "[]");

		parser.addArg<mvPyDataType::Bool>("contribute_to_bounds", mvArgType::KEYWORD_ARG, "True");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvShadeSeries::mvShadeSeries(mvUUID uuid)
		: mvSeriesBase(uuid)
	{
		m_contributeToBounds = true;
	}

	void mvShadeSeries::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(m_uuid);
		mvImPlotThemeScope scope(this);

		static const std::vector<double>* xptr;
		static const std::vector<double>* y1ptr;
		static const std::vector<double>* y2ptr;

		xptr = &(*m_value.get())[0];
		y1ptr = &(*m_value.get())[1];
		y2ptr = &(*m_value.get())[2];

		ImPlot::PlotShaded(m_label.c_str(), xptr->data(), y1ptr->data(),
			y2ptr->data(), (int)xptr->size());

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

	void mvShadeSeries::handleSpecificRequiredArgs(PyObject* dict)
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
				(*m_value)[2] = ToDoubleVect(item);
				break;

			default:
				break;
			}
		}

		for (auto& item : (*m_value)[2])
			item = 0.0;

		resetMaxMins();
		calculateMaxMins();
	}

	void mvShadeSeries::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "contribute_to_bounds")) m_contributeToBounds = ToBool(item);

		bool valueChanged = false;
		if (PyObject* item = PyDict_GetItemString(dict, "x")) { valueChanged = true; (*m_value)[0] = ToDoubleVect(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "y1")) { valueChanged = true; (*m_value)[1] = ToDoubleVect(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "y2")) { valueChanged = true; (*m_value)[2] = ToDoubleVect(item); }

		if (valueChanged)
		{
			if ((*m_value)[1].size() != (*m_value)[2].size())
			{
				(*m_value)[2].clear();
				for (auto& item : (*m_value)[1])
					(*m_value)[2].push_back(0.0);
			}
			resetMaxMins();
			calculateMaxMins();
		}
	}

	void mvShadeSeries::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "contribute_to_bounds", ToPyBool(m_contributeToBounds));
	}

}