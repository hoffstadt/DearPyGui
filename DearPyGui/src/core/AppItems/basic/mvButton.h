#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvButton Keyword Struct
	//-----------------------------------------------------------------------------
	struct mvButtonConfig : public mvAppItemConfig
	{
		ImGuiDir direction = ImGuiDir_Up;
		bool small_button = false;
		bool arrow = false;
	};

	//-----------------------------------------------------------------------------
	// mvButton
	//-----------------------------------------------------------------------------
	MV_REGISTER_WIDGET(mvButton);
	class mvButton : public mvAppItem
	{

		MV_APPITEM_TYPE(mvAppItemType::mvButton, add_button)

		MV_CREATE_THEME_CONSTANT(mvDir_None,  -1L, 0L);
		MV_CREATE_THEME_CONSTANT(mvDir_Left,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvDir_Right,  1L, 0L);
		MV_CREATE_THEME_CONSTANT(mvDir_Up,  2L, 0L);
		MV_CREATE_THEME_CONSTANT(mvDir_Down,  3L, 0L);

		MV_CREATE_THEME_CONSTANT(mvThemeCol_Button_Text			,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Button_Bg			, 21L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Button_Hovered		, 22L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Button_Active		, 23L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Button_Border		,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Button_BorderShadow	,  6L, 0L);

		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Button_Rounding	, 11L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Button_BorderSize	, 12L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Button_TextAlignX	, 22L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Button_TextAlignY	, 22L, 1L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Button_PaddingX	, 10L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Button_PaddingY	, 10L, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
			MV_CREATE_CONSTANT_SINGLE(mvDir_None),
			MV_CREATE_CONSTANT_SINGLE(mvDir_Left),
			MV_CREATE_CONSTANT_SINGLE(mvDir_Right),
			MV_CREATE_CONSTANT_SINGLE(mvDir_Up),
			MV_CREATE_CONSTANT_SINGLE(mvDir_Down),
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeCol_Button_Text,         mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
			MV_CREATE_CONSTANT_TUPLE(mvThemeCol_Button_Bg,           mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
			MV_CREATE_CONSTANT_TUPLE(mvThemeCol_Button_Hovered,      mvColor( 66, 150, 250, 102), mvColor(128, 128, 128, 63)),
			MV_CREATE_CONSTANT_TUPLE(mvThemeCol_Button_Active,       mvColor( 66, 150, 250, 171), mvColor(128, 128, 128, 63)),
			MV_CREATE_CONSTANT_TUPLE(mvThemeCol_Button_Border,       mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
			MV_CREATE_CONSTANT_TUPLE(mvThemeCol_Button_BorderShadow, mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Button_Rounding	, 0    ,12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Button_BorderSize	, 0    , 1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Button_TextAlignX	, 0.5f , 1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Button_TextAlignY	, 0.5f , 1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Button_PaddingX	, 4    ,20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Button_PaddingY	, 3    ,20),
		MV_END_STYLE_CONSTANTS


	public:

		static void InsertParser   (std::map<std::string, mvPythonParser>* parsers);

	public:

		mvButton(const std::string& name);
		mvButton(const std::string& name, const mvButtonConfig& config);

		void draw() override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

		// cpp interface
		void             updateConfig(mvAppItemConfig* config) override;
		mvAppItemConfig* getConfig   () override;

	private:

		mvButtonConfig m_config;

	};

}
