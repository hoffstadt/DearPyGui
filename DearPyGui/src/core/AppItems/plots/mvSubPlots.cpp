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
		//_label = "SubPlot###" + std::to_string(_uuid);
		_width = -1;
		_height = -1;
	}

	void mvSubPlots::onChildAdd(mvRef<mvAppItem> item)
	{
		if (item->getType() == mvAppItemType::mvPlotLegend)
		{
			_flags &= ~ImPlotSubplotFlags_NoLegend;
			_flags |= ImPlotSubplotFlags_ShareItems;
		}
	}

	void mvSubPlots::onChildRemoved(mvRef<mvAppItem> item)
	{

		if (item->getType() == mvAppItemType::mvPlotLegend)
		{
			_flags |= ImPlotSubplotFlags_NoLegend;
			_flags &= ~ImPlotSubplotFlags_ShareItems;
		}

	}

	void mvSubPlots::addFlag(ImPlotSubplotFlags flag)
	{
		_flags |= flag;
	}

	void mvSubPlots::removeFlag(ImPlotSubplotFlags flag)
	{
		_flags &= ~flag;
	}

	void mvSubPlots::draw(ImDrawList* drawlist, float x, float y)
	{

		ScopedID id(_uuid);

		if (ImPlot::BeginSubplots(_internalLabel.c_str(),  _rows, _cols, ImVec2((float)_width, (float)_height),
			_flags, _row_ratios.empty() ? nullptr : _row_ratios.data(), _col_ratios.empty() ? nullptr : _col_ratios.data()))
		{

			// plots
			for (auto& item : _children[1])
			{
				// skip item if it's not shown
				if (!item->isShown())
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
				_rows = ToInt(item);
				break;

			case 1:
				_cols = ToInt(item);
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

		if (PyObject* item = PyDict_GetItemString(dict, "rows")) _rows = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "columns")) _cols = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "row_ratios")) _row_ratios = ToFloatVect(item);
		if (PyObject* item = PyDict_GetItemString(dict, "column_ratios")) _col_ratios = ToFloatVect(item);

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// subplot flags
		flagop("no_title", ImPlotSubplotFlags_NoTitle, _flags);
		flagop("no_menus", ImPlotSubplotFlags_NoMenus, _flags);
		flagop("no_resize", ImPlotSubplotFlags_NoResize, _flags);
		flagop("no_align", ImPlotSubplotFlags_NoAlign, _flags);
		flagop("link_rows", ImPlotSubplotFlags_LinkRows, _flags);
		flagop("link_columns", ImPlotSubplotFlags_LinkCols, _flags);
		flagop("link_all_x", ImPlotSubplotFlags_LinkAllX, _flags);
		flagop("link_all_y", ImPlotSubplotFlags_LinkAllY, _flags);
		flagop("column_major", ImPlotSubplotFlags_ColMajor, _flags);


	}

	void mvSubPlots::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "rows", ToPyInt(_rows));
		PyDict_SetItemString(dict, "cols", ToPyInt(_cols));
		PyDict_SetItemString(dict, "row_ratios", ToPyList(_row_ratios));
		PyDict_SetItemString(dict, "column_ratios", ToPyList(_col_ratios));

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		// subplot flags
		checkbitset("no_title", ImPlotSubplotFlags_NoTitle, _flags);
		checkbitset("no_menus", ImPlotSubplotFlags_NoMenus, _flags);
		checkbitset("no_resize", ImPlotSubplotFlags_NoResize, _flags);
		checkbitset("no_align", ImPlotSubplotFlags_NoAlign, _flags);
		checkbitset("link_rows", ImPlotSubplotFlags_LinkRows, _flags);
		checkbitset("link_columns", ImPlotSubplotFlags_LinkCols, _flags);
		checkbitset("link_all_x", ImPlotSubplotFlags_LinkAllX, _flags);
		checkbitset("link_all_y", ImPlotSubplotFlags_LinkAllY, _flags);
		checkbitset("column_major", ImPlotSubplotFlags_ColMajor, _flags);
	}

}
