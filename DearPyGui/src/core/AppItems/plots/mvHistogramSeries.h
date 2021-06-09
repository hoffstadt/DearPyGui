#pragma once

#include "mvPlot.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvHistogramSeries, MV_ITEM_DESC_DEFAULT | MV_ITEM_DESC_CONTAINER, StorageValueTypes::Series, 1);
	class mvHistogramSeries : public mvSeriesBase
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvHistogramSeries, add_histogram_series)

		MV_CREATE_CONSTANT(mvPlotBin_Sqrt,    -1L, 0L);
		MV_CREATE_CONSTANT(mvPlotBin_Sturges, -2L, 0L);
		MV_CREATE_CONSTANT(mvPlotBin_Rice,    -3L, 0L);
		MV_CREATE_CONSTANT(mvPlotBin_Scott,   -4L, 0L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
			MV_ADD_GENERAL_CONSTANT(mvPlotBin_Sqrt),
			MV_ADD_GENERAL_CONSTANT(mvPlotBin_Sturges),
			MV_ADD_GENERAL_CONSTANT(mvPlotBin_Rice),
			MV_ADD_GENERAL_CONSTANT(mvPlotBin_Scott),
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

		mvHistogramSeries(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificRequiredArgs(PyObject* args) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		int m_bins = -1;
		bool m_cumlative = false;
		bool m_density = false;
		bool m_outliers = true;
		float m_barScale = 1.0f;
		double m_min = 0.0;
		double m_max = 1.0;

	};

}
