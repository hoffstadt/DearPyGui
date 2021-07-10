#include <utility>
#include "mv2dHistogramSeries.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"

namespace Marvel {

	void mv2dHistogramSeries::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Plotting", "Containers", "Widgets"});
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::DoubleList>("x");
		parser.addArg<mvPyDataType::DoubleList>("y");


		parser.addArg<mvPyDataType::Integer>("xbins", mvArgType::KEYWORD_ARG, "-1");
		parser.addArg<mvPyDataType::Integer>("ybins", mvArgType::KEYWORD_ARG, "-1");
		parser.addArg<mvPyDataType::Double>("xmin_range", mvArgType::KEYWORD_ARG, "0.0");
		parser.addArg<mvPyDataType::Double>("xmax_range", mvArgType::KEYWORD_ARG, "1.0");
		parser.addArg<mvPyDataType::Double>("ymin_range", mvArgType::KEYWORD_ARG, "0.0");
		parser.addArg<mvPyDataType::Double>("ymax_range", mvArgType::KEYWORD_ARG, "1.0");
		parser.addArg<mvPyDataType::Bool>("density", mvArgType::KEYWORD_ARG, "False");
		parser.addArg<mvPyDataType::Bool>("outliers", mvArgType::KEYWORD_ARG, "True");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mv2dHistogramSeries::mv2dHistogramSeries(mvUUID uuid)
		: mvSeriesBase(uuid)
	{
	}

	bool mv2dHistogramSeries::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvPlotAxis) return true;

		mvThrowPythonError(mvErrorCode::mvIncompatibleParent, s_command,
			"Incompatible parent. Acceptable parents include: plot axis", this);

		assert(false);
		return false;
	}

	void mv2dHistogramSeries::draw(ImDrawList* drawlist, float x, float y)
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

			static const std::vector<double>* xptr;
			static const std::vector<double>* yptr;

			xptr = &(*m_value.get())[0];
			yptr = &(*m_value.get())[1];

			ImPlot::PlotHistogram2D(m_label.c_str(), xptr->data(), yptr->data(), (int)xptr->size(),
				m_xbins, m_ybins, m_density, ImPlotLimits(m_xmin, m_xmax, m_ymin, m_ymax), m_outliers);

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

	void mv2dHistogramSeries::handleSpecificRequiredArgs(PyObject* dict)
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

	void mv2dHistogramSeries::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "x")) { (*m_value)[0] = ToDoubleVect(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "xbins")) { m_xbins = ToInt(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "ybins")) { m_ybins = ToInt(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "xmin_range")) { m_xmin = ToDouble(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "xmax_range")) { m_xmax = ToDouble(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "ymin_range")) { m_ymin = ToDouble(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "ymax_range")) { m_ymax = ToDouble(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "density")) { m_density = ToBool(item); }
		if (PyObject* item = PyDict_GetItemString(dict, "outliers")) { m_outliers = ToBool(item); }

	}

	void mv2dHistogramSeries::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	}

}