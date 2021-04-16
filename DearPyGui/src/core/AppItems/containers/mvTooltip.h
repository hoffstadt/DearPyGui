#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvTooltip, MV_ITEM_DESC_CONTAINER | MV_ITEM_DESC_AFTER, StorageValueTypes::Bool, 1);
	class mvTooltip : public mvBoolPtrBase
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTooltip, add_tooltip)

		MV_CREATE_CONSTANT(mvThemeCol_Tooltip_Bg, ImGuiCol_PopupBg, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Tooltip_Border, ImGuiCol_Border, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_Tooltip_BorderSize, ImGuiStyleVar_WindowBorderSize, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Tooltip_Rounding, ImGuiStyleVar_WindowRounding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Tooltip_PaddingX, ImGuiStyleVar_WindowPadding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Tooltip_PaddingY, ImGuiStyleVar_WindowPadding, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Tooltip_ItemSpacingX, ImGuiStyleVar_ItemSpacing, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Tooltip_ItemSpacingY, ImGuiStyleVar_ItemSpacing, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Tooltip_Bg, mvImGuiCol_PopupBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Tooltip_Border, mvImGuiCol_Border),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT_F(mvThemeStyle_Tooltip_BorderSize, 1, 1),
			MV_ADD_CONSTANT_F(mvThemeStyle_Tooltip_Rounding, 0, 12),
			MV_ADD_CONSTANT_F(mvThemeStyle_Tooltip_PaddingX, 4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Tooltip_PaddingY, 3, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Tooltip_ItemSpacingX, 8, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Tooltip_ItemSpacingY, 8, 20),
		MV_END_STYLE_CONSTANTS

	public:

		mvTooltip(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y) override;

	};

}