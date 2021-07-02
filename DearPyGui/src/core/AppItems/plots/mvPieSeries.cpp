#include <utility>
#include "mvPieSeries.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"

namespace Marvel {

	void mvPieSeries::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a pie series to a plot.", { "Plotting", "Containers", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::Double>("x");
		parser.addArg<mvPyDataType::Double>("y");
		parser.addArg<mvPyDataType::Double>("radius");
		parser.addArg<mvPyDataType::DoubleList>("values");
		parser.addArg<mvPyDataType::StringList>("labels");

		parser.addArg<mvPyDataType::String>("format", mvArgType::KEYWORD_ARG, "'%0.2f'");

		parser.addArg<mvPyDataType::Double>("angle", mvArgType::KEYWORD_ARG, "90.0");

		parser.addArg<mvPyDataType::Bool>("normalize", mvArgType::KEYWORD_ARG, "False");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvPieSeries::mvPieSeries(mvUUID uuid)
		: mvSeriesBase(uuid)
	{
	}

	bool mvPieSeries::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvPlotAxis) return true;

		mvThrowPythonError(mvErrorCode::mvIncompatibleParent, s_command,
			"Incompatible parent. Acceptable parents include: plot axis", this);

		assert(false);
		return false;
	}

	void mvPieSeries::draw(ImDrawList* drawlist, float x, float y)
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

			ImPlot::PlotPieChart(m_clabels.data(), xptr->data(), (int)m_labels.size(),
				m_x, m_y, m_radius, m_normalize, m_format.c_str(), m_angle);

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
				m_x = ToDouble(item);
				break;

			case 1:
				m_y = ToDouble(item);
				break;

			case 2:
				m_radius = ToDouble(item);
				break;

			case 3:
				(*m_value)[0] = ToDoubleVect(item);
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
	}

	void mvPieSeries::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "format")) m_format = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "x")) m_x = ToDouble(item);
		if (PyObject* item = PyDict_GetItemString(dict, "y")) m_y = ToDouble(item);
		if (PyObject* item = PyDict_GetItemString(dict, "radius")) m_radius = ToDouble(item);
		if (PyObject* item = PyDict_GetItemString(dict, "angle")) m_angle = ToDouble(item);
		if (PyObject* item = PyDict_GetItemString(dict, "normalize")) m_normalize = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "labels"))
		{
			m_labels = ToStringVect(item);
			m_clabels.clear();
			for (const auto& label : m_labels)
				m_clabels.push_back(label.c_str());
		}

		if (PyObject* item = PyDict_GetItemString(dict, "values")) { (*m_value)[0] = ToDoubleVect(item); }


	}

	void mvPieSeries::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "format", ToPyString(m_format));
		PyDict_SetItemString(dict, "x", ToPyDouble(m_x));
		PyDict_SetItemString(dict, "y", ToPyDouble(m_y));
		PyDict_SetItemString(dict, "radius", ToPyDouble(m_radius));
		PyDict_SetItemString(dict, "angle", ToPyDouble(m_angle));
		PyDict_SetItemString(dict, "normalize", ToPyBool(m_normalize));
		PyDict_SetItemString(dict, "labels", ToPyList(m_labels));
	}

}