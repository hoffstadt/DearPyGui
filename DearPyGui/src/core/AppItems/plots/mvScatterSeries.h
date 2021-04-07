#pragma once

#include "mvPlot.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvScatterSeries, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Series, 1);
	class mvScatterSeries : public mvSeriesBase
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION2(mvAppItemType::mvScatterSeries, add_scatter_series)

		MV_CREATE_CONSTANT(mvThemeCol_Plot_Scatter_Outline, ImPlotCol_MarkerOutline, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Plot_Scatter_Fill, ImPlotCol_MarkerFill, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_Plot_Scatter_Marker, ImPlotStyleVar_Marker, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_Scatter_Size, ImPlotStyleVar_MarkerSize, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_Scatter_Weight, ImPlotStyleVar_MarkerWeight, 0L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_ADD_CONSTANT(mvThemeCol_Plot_Scatter_Outline, mvColor(0, 0, 0, -255), mvColor(0, 0, 0, -255)),
		MV_ADD_CONSTANT(mvThemeCol_Plot_Scatter_Fill, mvColor(0, 0, 0, -255), mvColor(0, 0, 0, -255)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_ADD_CONSTANT(mvThemeStyle_Plot_Scatter_Marker, ImPlotMarker_Circle, 9),
		MV_ADD_CONSTANT(mvThemeStyle_Plot_Scatter_Size, 4.0f, 12),
		MV_ADD_CONSTANT(mvThemeStyle_Plot_Scatter_Weight, 1.0f, 12),
		MV_END_STYLE_CONSTANTS

	public:

		mvScatterSeries(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	};

}
