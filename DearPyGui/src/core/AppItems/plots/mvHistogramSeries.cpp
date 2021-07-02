#include <utility>
#include "mvHistogramSeries.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"

namespace Marvel {

	void mvHistogramSeries::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a histogram series to a plot.", { "Plotting", "Containers", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
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
	}

	bool mvHistogramSeries::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvPlotAxis) return true;

		mvThrowPythonError(mvErrorCode::mvIncompatibleParent, s_command,
			"Incompatible parent. Acceptable parents include: plot axis", this);

		assert(false);
		return false;
	}

	void mvHistogramSeries::draw(ImDrawList* drawlist, float x, float y)
	{

		//-----------------------------------------------------------------------------
		// pre draw
		//-----------------------------------------------------------------------------
		if (!m_show)
			return;

		// push font if a font object is attached
		if (m_font)
		{
			ImFont* fontptr = static_cast<mvFont*>(m_font.get())->getFontPtr();
			ImGui::PushFont(fontptr);
		}

		// handle enabled theming
		if (m_enabled)
		{
			// push class theme (if it exists)
			if (auto classTheme = getClassTheme())
				static_cast<mvTheme*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);

			// push item theme (if it exists)
			if (m_theme)
				static_cast<mvTheme*>(m_theme.get())->draw(nullptr, 0.0f, 0.0f);
		}

		//-----------------------------------------------------------------------------
		// draw
		//-----------------------------------------------------------------------------
		{
			ScopedID id(m_uuid);

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


		//-----------------------------------------------------------------------------
		// update state
		//   * only update if applicable
		//-----------------------------------------------------------------------------


		//-----------------------------------------------------------------------------
		// post draw
		//-----------------------------------------------------------------------------

		// pop font off stack
		if (m_font)
			ImGui::PopFont();

		// handle popping styles
		if (m_enabled)
		{
			if (auto classTheme = getClassTheme())
				static_cast<mvTheme*>(classTheme.get())->customAction();

			if (m_theme)
				static_cast<mvTheme*>(m_theme.get())->customAction();
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
	}

	void mvHistogramSeries::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "x")) { (*m_value)[0] = ToDoubleVect(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "bins")) { m_bins = ToInt(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "bar_scale")) { m_barScale = ToFloat(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "min_range")) { m_min = ToDouble(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "max_range")) { m_max = ToDouble(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "cumlative")) { m_cumlative = ToBool(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "density")) { m_density = ToBool(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "outliers")) { m_outliers = ToBool(item); }

	}

	void mvHistogramSeries::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	}

}