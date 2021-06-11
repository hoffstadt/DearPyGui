#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvPlotLegend, MV_ITEM_DESC_DEFAULT | MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 0);
	class mvPlotLegend : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvPlotLegend, add_plot_legend)

		MV_CREATE_CONSTANT(mvPlot_Location_Center, ImPlotLocation_Center);
		MV_CREATE_CONSTANT(mvPlot_Location_North, ImPlotLocation_North);
		MV_CREATE_CONSTANT(mvPlot_Location_South, ImPlotLocation_South);
		MV_CREATE_CONSTANT(mvPlot_Location_West, ImPlotLocation_West);
		MV_CREATE_CONSTANT(mvPlot_Location_East, ImPlotLocation_East);
		MV_CREATE_CONSTANT(mvPlot_Location_NorthWest, ImPlotLocation_NorthWest);
		MV_CREATE_CONSTANT(mvPlot_Location_NorthEast, ImPlotLocation_NorthEast);
		MV_CREATE_CONSTANT(mvPlot_Location_SouthWest, ImPlotLocation_SouthWest);
		MV_CREATE_CONSTANT(mvPlot_Location_SouthEast, ImPlotLocation_SouthEast);

		MV_START_COMMANDS
		MV_END_COMMANDS

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

		mvPlotLegend(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		bool isParentCompatible(mvAppItemType type) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void hide() override;
		void show() override;
		void postDraw() override;

	private:

		ImPlotLocation m_legendLocation = ImPlotLocation_NorthWest;
		bool           m_horizontal = false;
		bool           m_outside = false;
	};

}
