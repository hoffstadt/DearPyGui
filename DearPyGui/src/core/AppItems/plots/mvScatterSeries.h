#pragma once

#include "mvPlot.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvScatterSeries, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Series, 1);
	class mvScatterSeries : public mvSeriesBase
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvScatterSeries, add_scatter_series)

		MV_CREATE_CONSTANT(mvThemeCol_Plot_Scatter_MarkerOutline, ImPlotCol_MarkerOutline, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Plot_Scatter_MarkerFill, ImPlotCol_MarkerFill, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_Plot_Scatter_Marker, ImPlotStyleVar_Marker, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_Scatter_MarkerSize, ImPlotStyleVar_MarkerSize, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_Scatter_MarkerWeight, ImPlotStyleVar_MarkerWeight, 0L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_ADD_CONSTANT(mvThemeCol_Plot_Scatter_MarkerOutline, mvColor(0, 0, 0, -255), mvColor(0, 0, 0, -255)),
		MV_ADD_CONSTANT(mvThemeCol_Plot_Scatter_MarkerFill, mvColor(0, 0, 0, -255), mvColor(0, 0, 0, -255)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_ADD_CONSTANT_F(mvThemeStyle_Plot_Scatter_Marker, ImPlotMarker_Circle, 9),
		MV_ADD_CONSTANT_F(mvThemeStyle_Plot_Scatter_MarkerSize, 4.0f, 12),
		MV_ADD_CONSTANT_F(mvThemeStyle_Plot_Scatter_MarkerWeight, 1.0f, 12),
		MV_END_STYLE_CONSTANTS

	public:

		mvScatterSeries(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificRequiredArgs(PyObject* args) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	};

}
