#pragma once

#include "mvPlot.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvStemSeries, MV_ITEM_DESC_DEFAULT | MV_ITEM_DESC_CONTAINER, StorageValueTypes::Series, 1);
	class mvStemSeries : public mvSeriesBase
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvStemSeries, add_stem_series)

		MV_CREATE_CONSTANT(mvThemeCol_Plot_Stem_Line, ImPlotCol_Line, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Plot_Stem_MarkerOutline, ImPlotCol_MarkerOutline, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Plot_Stem_MarkerFill, ImPlotCol_MarkerFill, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_Plot_Stem_Weight, ImPlotStyleVar_LineWeight, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_Stem_Marker, ImPlotStyleVar_Marker, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_Stem_MarkerWeight, ImPlotStyleVar_MarkerWeight, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_Stem_MarkerSize, ImPlotStyleVar_MarkerSize, 0L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_ADD_CONSTANT(mvThemeCol_Plot_Stem_Line, mvColor(0, 0, 0, -255), mvColor(0, 0, 0, -255)),
		MV_ADD_CONSTANT(mvThemeCol_Plot_Stem_MarkerOutline, mvColor(0, 0, 0, -255), mvColor(0, 0, 0, -255)),
		MV_ADD_CONSTANT(mvThemeCol_Plot_Stem_MarkerFill, mvColor(0, 0, 0, -255), mvColor(0, 0, 0, -255)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_ADD_CONSTANT_F(mvThemeStyle_Plot_Stem_Weight, 1.0f, 12),
		MV_ADD_CONSTANT_F(mvThemeStyle_Plot_Stem_Marker, ImPlotMarker_Diamond, 9),
		MV_ADD_CONSTANT_F(mvThemeStyle_Plot_Stem_MarkerWeight, 1.0f, 12),
		MV_ADD_CONSTANT_F(mvThemeStyle_Plot_Stem_MarkerSize, 4.0f, 12),
		MV_END_STYLE_CONSTANTS

	public:

		mvStemSeries(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificRequiredArgs(PyObject* args) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	};

}
