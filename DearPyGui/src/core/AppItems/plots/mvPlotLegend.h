#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvPlotLegend, MV_ITEM_DESC_DEFAULT | MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 0);
	class mvPlotLegend : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvPlotLegend, add_plot_legend)
		MV_NO_COMMANDS
		MV_DEFAULT_CHILDREN

		MV_CREATE_CONSTANT(mvPlot_Location_Center, ImPlotLocation_Center);
		MV_CREATE_CONSTANT(mvPlot_Location_North, ImPlotLocation_North);
		MV_CREATE_CONSTANT(mvPlot_Location_South, ImPlotLocation_South);
		MV_CREATE_CONSTANT(mvPlot_Location_West, ImPlotLocation_West);
		MV_CREATE_CONSTANT(mvPlot_Location_East, ImPlotLocation_East);
		MV_CREATE_CONSTANT(mvPlot_Location_NorthWest, ImPlotLocation_NorthWest);
		MV_CREATE_CONSTANT(mvPlot_Location_NorthEast, ImPlotLocation_NorthEast);
		MV_CREATE_CONSTANT(mvPlot_Location_SouthWest, ImPlotLocation_SouthWest);
		MV_CREATE_CONSTANT(mvPlot_Location_SouthEast, ImPlotLocation_SouthEast);

		MV_SET_STATES(MV_STATE_NONE);

		MV_START_PARENTS
			MV_ADD_PARENT(mvAppItemType::mvStage),
			MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
			MV_ADD_PARENT(mvAppItemType::mvPlot),
			MV_ADD_PARENT(mvAppItemType::mvSubPlots)
		MV_END_PARENTS

		MV_START_CONSTANTS
			MV_ADD_CONSTANT(mvPlot_Location_Center),
			MV_ADD_CONSTANT(mvPlot_Location_North),
			MV_ADD_CONSTANT(mvPlot_Location_South),
			MV_ADD_CONSTANT(mvPlot_Location_West),
			MV_ADD_CONSTANT(mvPlot_Location_East),
			MV_ADD_CONSTANT(mvPlot_Location_NorthWest),
			MV_ADD_CONSTANT(mvPlot_Location_NorthEast),
			MV_ADD_CONSTANT(mvPlot_Location_SouthWest),
			MV_ADD_CONSTANT(mvPlot_Location_SouthEast)
		MV_END_CONSTANTS

	public:

		explicit mvPlotLegend(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void applySpecificTemplate(mvAppItem* item) override;

	private:

		ImPlotLocation _legendLocation = ImPlotLocation_NorthWest;
		bool           _horizontal = false;
		bool           _outside = false;
	};

}
