#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_tooltip(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvTooltip : public mvBoolPtrBase
	{
		MV_APPITEM_TYPE(mvAppItemType::Tooltip, mvTooltip, "add_tooltip")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::Tooltip, mvThemeCol_Tooltip_Bg				,  4L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Tooltip, mvThemeCol_Tooltip_Border			,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Tooltip, mvThemeStyle_Tooltip_BorderSize	,  3L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Tooltip, mvThemeStyle_Tooltip_Rounding		,  2L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Tooltip, mvThemeStyle_Tooltip_PaddingX		,  1L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Tooltip, mvThemeStyle_Tooltip_PaddingY		,  1L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Tooltip, mvThemeStyle_Tooltip_ItemSpacingX	, 13L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Tooltip, mvThemeStyle_Tooltip_ItemSpacingY	, 13L, 1L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Tooltip_Bg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Tooltip_Border),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Tooltip_BorderSize	, 0,  1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Tooltip_Rounding		, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Tooltip_PaddingX		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Tooltip_PaddingY		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Tooltip_ItemSpacingX	, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Tooltip_ItemSpacingY	, 0, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvTooltip(const std::string& name);

		void draw() override;

	};

}