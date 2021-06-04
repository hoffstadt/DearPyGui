#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvPlotLegend, MV_ITEM_DESC_DEFAULT | MV_ITEM_DESC_CONTAINER, StorageValueTypes::None, 0);
	class mvPlotLegend : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvPlotLegend, add_plot_legend)

		MV_CREATE_CONSTANT(mvThemeCol_Plot_LegendBg, 8L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Plot_LegendBorder, 9L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Plot_LegendText, 10L, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_Plot_LegendPaddingX, 19L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_LegendPaddingY, 19L, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_LegendInnerPaddingX, 20L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_LegendInnerPaddingY, 20L, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_LegendSpacingX, 21L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Plot_LegendSpacingY, 21L, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_LegendBg, mvColor(20, 20, 20, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_LegendBorder, mvImGuiCol_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Plot_LegendText, mvImGuiCol_Text),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_LegendPaddingX, 10, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_LegendPaddingY, 10, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_LegendInnerPaddingX, 5, 10),
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_LegendInnerPaddingY, 5, 10),
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_LegendSpacingX, 5, 10),
			MV_ADD_CONSTANT_F(mvThemeStyle_Plot_LegendSpacingY, 0, 10),
		MV_END_STYLE_CONSTANTS

	public:

		mvPlotLegend(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y) override;
		bool isParentCompatible(mvAppItemType type) override;
		void hide() override;
		void show() override;
		void postDraw() override;

	};

}
