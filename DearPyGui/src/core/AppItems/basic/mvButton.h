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

#ifdef MV_CPP
	void add_button(const char* name, const mvButtonConfig& config = {});
	void add_button(const char* name, mvCallable callable);
#else
	PyObject* add_button(PyObject* self, PyObject* args, PyObject* kwargs);
#endif
	
	//-----------------------------------------------------------------------------
	// mvButton
	//-----------------------------------------------------------------------------
	class mvButton : public mvAppItem
	{

		MV_APPITEM_TYPE(mvAppItemType::Button, mvButton, "add_button")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::Button, mvThemeCol_Button_Text			,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Button, mvThemeCol_Button_Bg			, 21L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Button, mvThemeCol_Button_Hovered		, 22L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Button, mvThemeCol_Button_Active		, 23L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Button, mvThemeCol_Button_Border		,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Button, mvThemeCol_Button_BorderShadow	,  6L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Button, mvThemeStyle_Button_Rounding	, 11L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Button, mvThemeStyle_Button_BorderSize	, 12L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Button, mvThemeStyle_Button_TextAlignX	, 22L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Button, mvThemeStyle_Button_TextAlignY	, 22L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Button, mvThemeStyle_Button_PaddingX	, 10L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Button, mvThemeStyle_Button_PaddingY	, 10L, 1L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Button_Text,         mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Button_Bg,           mvColor( 41,  74, 122, 138)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Button_Hovered,      mvColor( 66, 150, 250, 102)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Button_Active,       mvColor( 66, 150, 250, 171)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Button_Border,       mvColor(110, 110, 128, 128)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Button_BorderShadow, mvColor(  0,   0,   0,   0)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Button_Rounding	, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Button_BorderSize	, 0 , 1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Button_TextAlignX	, 0 , 1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Button_TextAlignY	, 0 , 1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Button_PaddingX	, 0 ,20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Button_PaddingY	, 0 ,20),
		MV_END_STYLE_CONSTANTS


	public:

		static void InsertParser   (std::map<std::string, mvPythonParser>* parsers);
		static void InsertConstants(std::vector<std::pair<std::string, long>>& constants);

	public:

		mvButton(const std::string& name);
		mvButton(const std::string& name, const mvButtonConfig& config);

		void draw() override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

		// cpp interface
		void             updateConfig(mvAppItemConfig* config) override;
		mvAppItemConfig* getConfig   () override;

	private:

		mvButtonConfig m_config;

	};

}
