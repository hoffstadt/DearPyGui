#include <algorithm>
#include "mvSubPlots.h"
#include "mvPlotLegend.h"
#include "mvApp.h"
#include "mvLog.h"
#include "mvInput.h"
#include "mvAreaSeries.h"
#include "mvBarSeries.h"
#include "mvCandleSeries.h"
#include "mvErrorSeries.h"
#include "mvHeatSeries.h"
#include "mvImageSeries.h"
#include "mvInfiniteLineSeries.h"
#include "mvLabelSeries.h"
#include "mvPieSeries.h"
#include "mvScatterSeries.h"
#include "mvShadeSeries.h"
#include "mvStairSeries.h"
#include "mvStemSeries.h"
#include "mvPythonExceptions.h"
#include "mvPlotAxis.h"
#include "themes/mvTheme.h"
#include "containers/mvDragPayload.h"

namespace Marvel {

	void mvSubPlots::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		{
			mvPythonParser parser(mvPyDataType::UUID, "Adds a plot which is used to hold series, and can be drawn to with draw commands.", { "Plotting", "Containers", "Widgets" }, true);
			mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
				MV_PARSER_ARG_ID |
				MV_PARSER_ARG_WIDTH |
				MV_PARSER_ARG_HEIGHT |
				MV_PARSER_ARG_INDENT |
				MV_PARSER_ARG_PARENT |
				MV_PARSER_ARG_BEFORE |
				MV_PARSER_ARG_SHOW |
				MV_PARSER_ARG_CALLBACK |
				MV_PARSER_ARG_SEARCH_DELAY |
				MV_PARSER_ARG_FILTER |
				MV_PARSER_ARG_TRACKED |
				MV_PARSER_ARG_POS)
			);

			parser.addArg<mvPyDataType::Integer>("rows");
			parser.addArg<mvPyDataType::Integer>("columns");

			parser.addArg<mvPyDataType::FloatList>("row_ratios", mvArgType::KEYWORD_ARG, "[]");
			parser.addArg<mvPyDataType::FloatList>("column_ratios", mvArgType::KEYWORD_ARG, "[]");

			// plot flags
			parser.addArg<mvPyDataType::Bool>("no_title", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Bool>("no_menus", mvArgType::KEYWORD_ARG, "False", "the user will not be able to open context menus with right-click");
			parser.addArg<mvPyDataType::Bool>("no_resize", mvArgType::KEYWORD_ARG, "False", "resize splitters between subplot cells will be not be provided");
			parser.addArg<mvPyDataType::Bool>("no_align", mvArgType::KEYWORD_ARG, "False", "subplot edges will not be aligned vertically or horizontally");
			parser.addArg<mvPyDataType::Bool>("link_rows", mvArgType::KEYWORD_ARG, "False", "link the y-axis limits of all plots in each row (does not apply auxiliary y-axes)");
			parser.addArg<mvPyDataType::Bool>("link_columns", mvArgType::KEYWORD_ARG, "False", "link the x-axis limits of all plots in each column");
			parser.addArg<mvPyDataType::Bool>("link_all_x", mvArgType::KEYWORD_ARG, "False", "link the x-axis limits in every plot in the subplot");
			parser.addArg<mvPyDataType::Bool>("link_all_y", mvArgType::KEYWORD_ARG, "False", "link the y-axis limits in every plot in the subplot (does not apply to auxiliary y-axes)");
			parser.addArg<mvPyDataType::Bool>("column_major", mvArgType::KEYWORD_ARG, "False", "subplots are added in column major order instead of the default row major order");

			parser.finalize();
			parsers->insert({ s_command, parser });
		}

	}

	mvSubPlots::mvSubPlots(mvUUID uuid)
		: mvAppItem(uuid)
	{
		m_label = "SubPlot###" + std::to_string(m_uuid);
		m_width = -1;
		m_height = -1;
	}

	void mvSubPlots::onChildAdd(mvRef<mvAppItem> item)
	{
		if (item->getType() == mvAppItemType::mvPlotLegend)
		{
			m_flags &= ~ImPlotSubplotFlags_NoLegend;
			m_flags |= ImPlotSubplotFlags_ShareItems;
		}
	}

	void mvSubPlots::onChildRemoved(mvRef<mvAppItem> item)
	{

		if (item->getType() == mvAppItemType::mvPlotLegend)
		{
			m_flags |= ImPlotSubplotFlags_NoLegend;
			m_flags &= ~ImPlotSubplotFlags_ShareItems;
		}

	}

	void mvSubPlots::addFlag(ImPlotSubplotFlags flag)
	{
		m_flags |= flag;
	}

	void mvSubPlots::removeFlag(ImPlotSubplotFlags flag)
	{
		m_flags &= ~flag;
	}

	bool mvSubPlots::canChildBeAdded(mvAppItemType type)
	{
		if (type == mvAppItemType::mvPlotLegend) return true;
		if (type == mvAppItemType::mvPlot) return true;

		mvThrowPythonError(mvErrorCode::mvIncompatibleChild, s_command,
			"Incompatible child. Acceptable children include: mvPlot, mvPlotLegend", this);

		MV_ITEM_REGISTRY_ERROR("SubPlots children must be compatible.");
		assert(false);

		return false;
	}

	void mvSubPlots::draw(ImDrawList* drawlist, float x, float y)
	{

		ScopedID id(m_uuid);

		if (ImPlot::BeginSubplots(m_label.c_str(),  m_rows, m_cols, ImVec2((float)m_width, (float)m_height),
			m_flags, m_row_ratios.empty() ? nullptr : m_row_ratios.data(), m_col_ratios.empty() ? nullptr : m_col_ratios.data()))
		{

			// plots
			for (auto& item : m_children[1])
			{
				// skip item if it's not shown
				if (!item->m_show)
					continue;
				item->draw(drawlist, x, y);

				item->postDraw();
			}

			ImPlot::EndSubplots();
		}
	}

	void mvSubPlots::handleSpecificRequiredArgs(PyObject* dict)
	{
		if (!mvApp::GetApp()->getParsers()[s_command].verifyRequiredArguments(dict))
			return;

		for (int i = 0; i < PyTuple_Size(dict); i++)
		{
			PyObject* item = PyTuple_GetItem(dict, i);
			switch (i)
			{
			case 0:
				m_rows = ToInt(item);
				break;

			case 1:
				m_cols = ToInt(item);
				break;

			default:
				break;
			}
		}
	}

	void mvSubPlots::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "rows")) m_rows = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "columns")) m_cols = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "row_ratios")) m_row_ratios = ToFloatVect(item);
		if (PyObject* item = PyDict_GetItemString(dict, "column_ratios")) m_col_ratios = ToFloatVect(item);

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// subplot flags
		flagop("no_title", ImPlotSubplotFlags_NoTitle, m_flags);
		flagop("no_menus", ImPlotSubplotFlags_NoMenus, m_flags);
		flagop("no_resize", ImPlotSubplotFlags_NoResize, m_flags);
		flagop("no_align", ImPlotSubplotFlags_NoAlign, m_flags);
		flagop("link_rows", ImPlotSubplotFlags_LinkRows, m_flags);
		flagop("link_columns", ImPlotSubplotFlags_LinkCols, m_flags);
		flagop("link_all_x", ImPlotSubplotFlags_LinkAllX, m_flags);
		flagop("link_all_y", ImPlotSubplotFlags_LinkAllY, m_flags);
		flagop("column_major", ImPlotSubplotFlags_ColMajor, m_flags);


	}

	void mvSubPlots::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "rows", ToPyInt(m_rows));
		PyDict_SetItemString(dict, "cols", ToPyInt(m_cols));
		PyDict_SetItemString(dict, "row_ratios", ToPyList(m_row_ratios));
		PyDict_SetItemString(dict, "column_ratios", ToPyList(m_col_ratios));

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		// subplot flags
		checkbitset("no_title", ImPlotSubplotFlags_NoTitle, m_flags);
		checkbitset("no_menus", ImPlotSubplotFlags_NoMenus, m_flags);
		checkbitset("no_resize", ImPlotSubplotFlags_NoResize, m_flags);
		checkbitset("no_align", ImPlotSubplotFlags_NoAlign, m_flags);
		checkbitset("link_rows", ImPlotSubplotFlags_LinkRows, m_flags);
		checkbitset("link_columns", ImPlotSubplotFlags_LinkCols, m_flags);
		checkbitset("link_all_x", ImPlotSubplotFlags_LinkAllX, m_flags);
		checkbitset("link_all_y", ImPlotSubplotFlags_LinkAllY, m_flags);
		checkbitset("column_major", ImPlotSubplotFlags_ColMajor, m_flags);
	}

}
