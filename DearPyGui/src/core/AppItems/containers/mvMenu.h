#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	struct mvMenuConfig : public mvAppItemConfig {};

	PyObject* add_menu(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvMenu : public mvBoolPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::Menu, mvMenu, "add_menu")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::Menu, mvThemeCol_Menu_Text			,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Menu, mvThemeCol_Menu_Bg			,  4L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Menu, mvThemeCol_Menu_BgHovered		, 25L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Menu, mvThemeCol_Menu_BgActive		, 24L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Menu, mvThemeCol_Menu_Border		,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Menu, mvThemeStyle_Menu_BorderSize	,  9L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Menu, mvThemeStyle_Menu_Rounding	,  8L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Menu, mvThemeStyle_Menu_PaddingX	,  1L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Menu, mvThemeStyle_Menu_PaddingY	,  1L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Menu, mvThemeStyle_Menu_ItemSpacingX, 13L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Menu, mvThemeStyle_Menu_ItemSpacingY, 13L, 1L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Menu_Text, mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Menu_Bg, mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Menu_BgHovered, mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Menu_BgActive, mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Menu_Border, mvColor(255, 255, 255, 255)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Menu_BorderSize	, 0,  1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Menu_Rounding		, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Menu_PaddingX		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Menu_PaddingY		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Menu_ItemSpacingX	, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Menu_ItemSpacingY	, 0, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvMenu(const std::string& name);

		void draw() override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	};

}