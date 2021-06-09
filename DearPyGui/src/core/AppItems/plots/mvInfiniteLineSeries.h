#pragma once

#include "mvPlot.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvVLineSeries
//     * mvHLineSeries
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvVLineSeries
	//-----------------------------------------------------------------------------
	MV_REGISTER_WIDGET(mvVLineSeries, MV_ITEM_DESC_DEFAULT | MV_ITEM_DESC_CONTAINER, StorageValueTypes::Series, 1);
	class mvVLineSeries : public mvSeriesBase
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvVLineSeries, add_vline_series)

		MV_CREATE_CONSTANT(mvThemeCol_Plot_VLine, ImPlotCol_Line, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_Plot_VLine_Weight, ImPlotStyleVar_LineWeight, 0L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_ADD_CONSTANT(mvThemeCol_Plot_VLine, mvColor(0, 0, 0, -255), mvColor(0, 0, 0, -255)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_ADD_CONSTANT_F(mvThemeStyle_Plot_VLine_Weight, 1.0f, 12),
		MV_END_STYLE_CONSTANTS

	public:

		mvVLineSeries(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificRequiredArgs(PyObject* args) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	};

	//-----------------------------------------------------------------------------
	// mvHLineSeries
	//-----------------------------------------------------------------------------
	MV_REGISTER_WIDGET(mvHLineSeries, MV_ITEM_DESC_DEFAULT | MV_ITEM_DESC_CONTAINER, StorageValueTypes::Series, 1);
	class mvHLineSeries : public mvSeriesBase
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvHLineSeries, add_hline_series)

		MV_CREATE_CONSTANT(mvThemeCol_Plot_HLine, ImPlotCol_Line, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_Plot_HLine_Weight, ImPlotStyleVar_LineWeight, 0L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_ADD_CONSTANT(mvThemeCol_Plot_HLine, mvColor(0, 0, 0, -255), mvColor(0, 0, 0, -255)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_ADD_CONSTANT_F(mvThemeStyle_Plot_HLine_Weight, 1.0f, 12),
		MV_END_STYLE_CONSTANTS

	public:

		mvHLineSeries(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificRequiredArgs(PyObject* args) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	};

}
