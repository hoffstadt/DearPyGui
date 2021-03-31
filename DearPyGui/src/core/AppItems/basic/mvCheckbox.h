#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvCheckbox Keyword Struct
	//-----------------------------------------------------------------------------
	struct mvCheckboxConfig : public mvAppItemConfig
	{
		bool default_value = false;
		std::string source = "";
	};

	PyObject* add_checkbox(PyObject* self, PyObject* args, PyObject* kwargs);

	//-----------------------------------------------------------------------------
	// mvCheckbox Keyword Struct
	//-----------------------------------------------------------------------------
	MV_REGISTER_WIDGET(mvCheckbox);
	class mvCheckbox : public mvBoolPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::mvCheckbox, "add_checkbox")

		MV_CREATE_THEME_CONSTANT(mvThemeCol_CheckBox_Text				,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_CheckBox_Bg					,  7L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_CheckBox_BgHovered			,  8L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_CheckBox_BgActive			, 18L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_CheckBox_Border				,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_CheckBox_BorderShadow		,  6L, 0L);

		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Checkbox_Rounding			, 11L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Checkbox_BorderSize		, 12L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Checkbox_PaddingX			, 10L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Checkbox_PaddingY			, 10L, 1L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Checkbox_InnerItemSpacingX, 14L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Checkbox_InnerItemSpacingY, 14L, 1L);

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeCol_CheckBox_Text,			mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
			MV_CREATE_CONSTANT_TUPLE(mvThemeCol_CheckBox_Bg,			mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
			MV_CREATE_CONSTANT_TUPLE(mvThemeCol_CheckBox_BgHovered,		mvColor( 66, 150, 250, 102), mvColor(128, 128, 128, 63)),
			MV_CREATE_CONSTANT_TUPLE(mvThemeCol_CheckBox_BgActive,		mvColor( 66, 150, 250, 171), mvColor(128, 128, 128, 63)),
			MV_CREATE_CONSTANT_TUPLE(mvThemeCol_CheckBox_Border,		mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
			MV_CREATE_CONSTANT_TUPLE(mvThemeCol_CheckBox_BorderShadow,	mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Checkbox_BorderSize		, 0,  1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Checkbox_Rounding			, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Checkbox_PaddingX			, 4, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Checkbox_PaddingY			, 3, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Checkbox_InnerItemSpacingX, 4, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Checkbox_InnerItemSpacingY, 4, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvCheckbox(const std::string& name, bool default_value, const std::string& dataSource);
		mvCheckbox(const std::string& name, const mvCheckboxConfig& config);

		void draw() override;

		// cpp interface
		void updateConfig(mvAppItemConfig* config) override;
		mvAppItemConfig* getConfig() override;

	private:

		mvCheckboxConfig m_config;

	};

}
