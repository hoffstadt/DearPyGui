#pragma once

#include "mvPlot.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvHeatSeries, MV_ITEM_DESC_DEFAULT | MV_ITEM_DESC_CONTAINER, StorageValueTypes::Series, 1);
	class mvHeatSeries : public mvAppItem
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvHeatSeries, add_heat_series)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvHeatSeries(mvUUID uuid);

		// overriding until we can remove these
		bool preDraw() override { return true; }
		void postDraw() override {}

		void draw(ImDrawList* drawlist, float x, float y) override;
		void setDataSource(mvUUID dataSource) override;
		mvValueVariant getValue() override { return _value; }
		PyObject* getPyValue() override;
		void setPyValue(PyObject* value) override;
		bool isParentCompatible(mvAppItemType type) override;
		void handleSpecificRequiredArgs(PyObject* args) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		int         _rows = 1;
		int         _cols = 1;
		double      _scale_min = 0.0;
		double      _scale_max = 1.0;
		std::string _format = "%0.1f";
		mvPlotPoint _bounds_min = {0.0, 0.0};
		mvPlotPoint _bounds_max = {1.0, 1.0};
		mvRef<std::vector<std::vector<double>>> _value = CreateRef<std::vector<std::vector<double>>>(
			std::vector<std::vector<double>>{ std::vector<double>{},
			std::vector<double>{},
			std::vector<double>{},
			std::vector<double>{},
			std::vector<double>{} });

	};

}
