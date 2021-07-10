#include <utility>
#include "mvHeatSeries.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"

namespace Marvel {

	void mvHeatSeries::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a heat series to a plot. Typically a color scale widget is also added to show the legend.", { "Plotting", "Containers", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::DoubleList>("x");
		parser.addArg<mvPyDataType::Integer>("rows");
		parser.addArg<mvPyDataType::Integer>("cols");


		parser.addArg<mvPyDataType::Double>("scale_min", mvArgType::KEYWORD_ARG, "0.0", "Sets the color scale min. Typically paired with the color scale widget scale_min.");
		parser.addArg<mvPyDataType::Double>("scale_max", mvArgType::KEYWORD_ARG, "1.0", "Sets the color scale max. Typically paired with the color scale widget scale_max.");

		parser.addArg<mvPyDataType::DoubleList>("bounds_min", mvArgType::KEYWORD_ARG, "(0.0, 0.0)");
		parser.addArg<mvPyDataType::DoubleList>("bounds_max", mvArgType::KEYWORD_ARG, "(1.0, 1.0)");

		parser.addArg<mvPyDataType::String>("format", mvArgType::KEYWORD_ARG, "'%0.1f'");

		parser.addArg<mvPyDataType::Bool>("contribute_to_bounds", mvArgType::KEYWORD_ARG, "True");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvHeatSeries::mvHeatSeries(mvUUID uuid)
		: mvSeriesBase(uuid)
	{
	}

	bool mvHeatSeries::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvPlotAxis) return true;

		mvThrowPythonError(mvErrorCode::mvIncompatibleParent, s_command,
			"Incompatible parent. Acceptable parents include: plot axis", this);

		assert(false);
		return false;
	}

	void mvHeatSeries::draw(ImDrawList* drawlist, float x, float y)
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

			xptr = &(*m_value.get())[0];


			ImPlot::PlotHeatmap(m_label.c_str(), xptr->data(), m_rows, m_cols, m_scale_min, m_scale_max,
				m_format.c_str(), { m_bounds_min.x, m_bounds_min.y }, { m_bounds_max.x, m_bounds_max.y });

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

		(*m_value)[1].push_back(m_bounds_min.y);
		(*m_value)[1].push_back(m_bounds_max.y);
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

		bool valueChanged = false;
		if (PyObject* item = PyDict_GetItemString(dict, "x")) { valueChanged = true; (*m_value)[0] = ToDoubleVect(item); }

		if (valueChanged)
		{
			(*m_value)[1].push_back(m_bounds_min.y);
			(*m_value)[1].push_back(m_bounds_max.y);
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
	}

}