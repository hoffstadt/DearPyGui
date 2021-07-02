#include <utility>
#include "mvInfiniteLineSeries.h"
#include "mvCore.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "AppItems/fonts/mvFont.h"
#include "AppItems/themes/mvTheme.h"
#include "AppItems/containers/mvDragPayload.h"

namespace Marvel {

	void mvVLineSeries::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a infinite vertical line series to a plot.", { "Plotting", "Containers", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::DoubleList>("x");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvVLineSeries::mvVLineSeries(mvUUID uuid)
		: mvSeriesBase(uuid)
	{
	}

	bool mvVLineSeries::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvPlotAxis) return true;

		mvThrowPythonError(mvErrorCode::mvIncompatibleParent, s_command,
			"Incompatible parent. Acceptable parents include: plot axis", this);

		assert(false);
		return false;
	}

	void mvVLineSeries::draw(ImDrawList* drawlist, float x, float y)
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

			ImPlot::PlotVLines(m_label.c_str(), xptr->data(), (int)xptr->size());

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

	void mvVLineSeries::handleSpecificRequiredArgs(PyObject* dict)
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

	void mvVLineSeries::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "x")) { (*m_value)[0] = ToDoubleVect(item); }


	}

	void mvVLineSeries::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	}

	void mvHLineSeries::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a infinite horizontal line series to a plot.", { "Plotting", "Containers", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SOURCE |
			MV_PARSER_ARG_SHOW)
		);

		parser.addArg<mvPyDataType::DoubleList>("x");

		parser.addArg<mvPyDataType::Bool>("contribute_to_bounds", mvArgType::KEYWORD_ARG, "True");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}

	mvHLineSeries::mvHLineSeries(mvUUID uuid)
		: mvSeriesBase(uuid)
	{
	}

	bool mvHLineSeries::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvPlotAxis) return true;

		mvThrowPythonError(mvErrorCode::mvIncompatibleParent, s_command,
			"Incompatible parent. Acceptable parents include: plot axis", this);

		assert(false);
		return false;
	}

	void mvHLineSeries::draw(ImDrawList* drawlist, float x, float y)
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

			ImPlot::PlotHLines(m_label.c_str(), xptr->data(), (int)xptr->size());

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

	void mvHLineSeries::handleSpecificRequiredArgs(PyObject* dict)
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

	void mvHLineSeries::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		bool valueChanged = false;
		if (PyObject* item = PyDict_GetItemString(dict, "x")) { valueChanged = true; (*m_value)[0] = ToDoubleVect(item); }

	}

	void mvHLineSeries::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
	}

}