#pragma once

#include "mvPlot.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvHistogramSeries, MV_ITEM_DESC_DEFAULT | MV_ITEM_DESC_CONTAINER, StorageValueTypes::Series, 1);
	class mvHistogramSeries : public mvSeriesBase
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvHistogramSeries, add_histogram_series)

		MV_CREATE_CONSTANT(mvPlotBin_Sqrt,    -1L);
		MV_CREATE_CONSTANT(mvPlotBin_Sturges, -2L);
		MV_CREATE_CONSTANT(mvPlotBin_Rice,    -3L);
		MV_CREATE_CONSTANT(mvPlotBin_Scott,   -4L);

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
			MV_ADD_CONSTANT(mvPlotBin_Sqrt),
			MV_ADD_CONSTANT(mvPlotBin_Sturges),
			MV_ADD_CONSTANT(mvPlotBin_Rice),
			MV_ADD_CONSTANT(mvPlotBin_Scott),
		MV_END_CONSTANTS

	public:

		mvHistogramSeries(mvUUID uuid);

		// overriding until we can remove these
		bool preDraw() override { return true; }
		void postDraw() override {}

		void draw(ImDrawList* drawlist, float x, float y) override;
		bool isParentCompatible(mvAppItemType type) override;
		void handleSpecificRequiredArgs(PyObject* args) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		int _bins = -1;
		bool _cumlative = false;
		bool _density = false;
		bool _outliers = true;
		float _barScale = 1.0f;
		double _min = 0.0;
		double _max = 1.0;

	};

}
