#pragma once

#include "mvPlot.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvErrorSeries, MV_ITEM_DESC_DEFAULT | MV_ITEM_DESC_CONTAINER, StorageValueTypes::Series, 1);
	class mvErrorSeries : public mvSeriesBase
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvErrorSeries, add_error_series)

		MV_CREATE_CONSTANT(mvThemeCol_Plot_Error, ImPlotCol_ErrorBar, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_Plot_Error_Size, ImPlotStyleVar_ErrorBarSize, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_Error_Weight, ImPlotStyleVar_ErrorBarWeight, 0L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_ADD_CONSTANT(mvThemeCol_Plot_Error, mvColor(0, 0, 0, -255), mvColor(0, 0, 0, -255)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_Error_Size, 5.0f, 10.0f),
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_Error_Weight, 1.5f, 10.0f),
		MV_END_STYLE_CONSTANTS

	public:

		mvErrorSeries(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificRequiredArgs(PyObject* args) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		bool m_horizontal = false;

	};

}
