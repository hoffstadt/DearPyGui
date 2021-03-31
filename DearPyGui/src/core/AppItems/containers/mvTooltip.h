#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	struct mvTooltipConfig : public mvAppItemConfig
	{
		std::string tipparent;
	};

	MV_REGISTER_WIDGET(mvTooltip);
	class mvTooltip : public mvBoolPtrBase
	{
		MV_APPITEM_TYPE(mvAppItemType::mvTooltip, add_tooltip)

		MV_CREATE_THEME_CONSTANT(mvThemeCol_Tooltip_Bg				,  4L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Tooltip_Border			,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Tooltip_BorderSize	,  3L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Tooltip_Rounding		,  2L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Tooltip_PaddingX		,  1L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Tooltip_PaddingY		,  1L, 1L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Tooltip_ItemSpacingX	, 13L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Tooltip_ItemSpacingY	, 13L, 1L);

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Tooltip_Bg,		mvColor( 20,  20,  20, 240)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Tooltip_Border,	mvColor(110, 110, 128, 128)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Tooltip_BorderSize	, 1,  1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Tooltip_Rounding		, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Tooltip_PaddingX		, 4, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Tooltip_PaddingY		, 3, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Tooltip_ItemSpacingX	, 8, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Tooltip_ItemSpacingY	, 8, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvTooltip(const std::string& name);

		void draw() override;

	};

}