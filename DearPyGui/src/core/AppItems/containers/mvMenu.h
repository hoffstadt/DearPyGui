#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	struct mvMenuConfig : public mvAppItemConfig {};

	MV_REGISTER_WIDGET(mvMenu);
	class mvMenu : public mvBoolPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::mvMenu, add_menu)

		MV_CREATE_THEME_CONSTANT(mvThemeCol_Menu_Text		    ,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Menu_Bg			    ,  4L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Menu_BgHovered	    , 25L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Menu_BgActive	    , 24L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Menu_Border		    ,  5L, 0L);

		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Menu_BorderSize	,  9L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Menu_Rounding	    ,  8L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Menu_PaddingX	    ,  1L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Menu_PaddingY	    ,  1L, 1L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Menu_ItemSpacingX , 13L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Menu_ItemSpacingY , 13L, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Menu_Text,		mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Menu_Bg,			mvColor( 20,  20,  20, 240)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Menu_BgHovered,	mvColor( 66, 150, 250, 204)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Menu_BgActive,	mvColor( 66, 150, 250, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Menu_Border,		mvColor(110, 110, 128, 128)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Menu_BorderSize	, 1,  1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Menu_Rounding		, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Menu_PaddingX		, 8, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Menu_PaddingY		, 8, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Menu_ItemSpacingX	, 8, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Menu_ItemSpacingY	, 4, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvMenu(const std::string& name);

		void draw() override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	};

}