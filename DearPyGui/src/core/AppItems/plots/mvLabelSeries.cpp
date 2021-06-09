#include <utility>
#include "mvLabelSeries.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImPlotThemeScope.h"

namespace Marvel {

	void mvLabelSeries::InsertParser(std::map<std::string, mvPythonParser>* parsers)
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

		parser.addArg<mvPyDataType::Double>("x");
		parser.addArg<mvPyDataType::Double>("y");
		parser.addArg<mvPyDataType::Integer>("x_offset", mvArgType::KEYWORD_ARG);
		parser.addArg<mvPyDataType::Integer>("y_offset", mvArgType::KEYWORD_ARG);

		parser.addArg<mvPyDataType::Bool>("contribute_to_bounds", mvArgType::KEYWORD_ARG, "True");
		parser.addArg<mvPyDataType::Bool>("vertical", mvArgType::KEYWORD_ARG, "False");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvLabelSeries::mvLabelSeries(mvUUID uuid)
		: mvSeriesBase(uuid)
	{
	}

	void mvLabelSeries::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(m_uuid);
		mvImPlotThemeScope scope(this);

		static const std::vector<double>* xptr;
		static const std::vector<double>* yptr;

		xptr = &(*m_value.get())[0];
		yptr = &(*m_value.get())[1];

		ImPlot::PlotText(m_label.c_str(), (*xptr)[0], (*yptr)[0], m_vertical,
			ImVec2((float)m_xoffset, (float)m_yoffset));

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

	void mvLabelSeries::handleSpecificRequiredArgs(PyObject* dict)
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

	void mvLabelSeries::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "contribute_to_bounds")) m_contributeToBounds = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "vertical")) m_vertical = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "x_offset")) m_xoffset = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "y_offset")) m_yoffset = ToInt(item);

		bool valueChanged = false;
		if (PyObject* item = PyDict_GetItemString(dict, "x")) { valueChanged = true; (*m_value)[0] = ToDoubleVect(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "y")) { valueChanged = true; (*m_value)[1] = ToDoubleVect(item); }

		if (valueChanged)
		{
			resetMaxMins();
			calculateMaxMins();
		}

	}

	void mvLabelSeries::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "contribute_to_bounds", ToPyBool(m_contributeToBounds));
		PyDict_SetItemString(dict, "vertical", ToPyBool(m_vertical));
		PyDict_SetItemString(dict, "x_offset", ToPyInt(m_xoffset));
		PyDict_SetItemString(dict, "y_offset", ToPyInt(m_yoffset));


	}

}