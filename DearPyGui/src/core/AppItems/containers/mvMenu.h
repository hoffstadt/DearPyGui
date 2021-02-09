#pragma once

#include "mvTypeBases.h"

namespace Marvel {

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
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Menu_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Menu_Bg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Menu_BgHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Menu_BgActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Menu_Border),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeStyle_Menu_BorderSize),
			MV_CREATE_CONSTANT_PAIR(mvThemeStyle_Menu_Rounding),
			MV_CREATE_CONSTANT_PAIR(mvThemeStyle_Menu_PaddingX),
			MV_CREATE_CONSTANT_PAIR(mvThemeStyle_Menu_PaddingY),
			MV_CREATE_CONSTANT_PAIR(mvThemeStyle_Menu_ItemSpacingX),
			MV_CREATE_CONSTANT_PAIR(mvThemeStyle_Menu_ItemSpacingY),
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