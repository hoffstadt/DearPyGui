#pragma once

#include "mvPlot.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvStairSeries, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Series, 1);
	class mvStairSeries : public mvSeriesBase
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvStairSeries, add_stair_series)

		MV_CREATE_CONSTANT(mvThemeCol_Plot_Stair, ImPlotCol_Line, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_Plot_Stair_Weight, ImPlotStyleVar_LineWeight, 0L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_ADD_CONSTANT(mvThemeCol_Plot_Stair, mvColor(0, 0, 0, -255), mvColor(0, 0, 0, -255)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_ADD_CONSTANT_F(mvThemeStyle_Plot_Stair_Weight, 1.0f, 12),
		MV_END_STYLE_CONSTANTS

	public:

		mvStairSeries(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	};

}
