#pragma once

#include "mvTypeBases.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvColorButton
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvColorButton Config Struct
	//-----------------------------------------------------------------------------
	struct mvColorButtonConfig : public mvAppItemConfig
	{
		mvColor color;
		bool no_alpha     = false;
		bool no_border    = false;
		bool no_drag_drop = false;
	};

	PyObject* add_color_button(PyObject* self, PyObject* args, PyObject* kwargs);

	MV_REGISTER_WIDGET(mvColorButton);
	class mvColorButton : public mvAppItem
	{
		MV_APPITEM_TYPE(mvAppItemType::mvColorButton, "add_color_button")

		MV_CREATE_THEME_CONSTANT(mvThemeCol_ColorButton_Text,              0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_ColorButton_Separator,        27L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_ColorButton_PopupBg,           4L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_ColorButton_Border,            5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_ColorButton_BorderShadow,      6L, 0L);

		MV_CREATE_THEME_CONSTANT(mvThemeStyle_ColorButton_Rounding,       11L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_ColorButton_BorderSize,     12L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_ColorButton_PopupRounding,   8L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_ColorButton_PopupBorderSize, 9L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_ColorButton_PopupPaddingX,   1L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_ColorButton_PopupPaddingY,   1L, 1L);

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeCol_ColorButton_Text,         mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
			MV_CREATE_CONSTANT_TUPLE(mvThemeCol_ColorButton_Separator,    mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
			MV_CREATE_CONSTANT_TUPLE(mvThemeCol_ColorButton_PopupBg,      mvColor( 20,  20,  20, 240), mvColor(128, 128, 128, 63)),
			MV_CREATE_CONSTANT_TUPLE(mvThemeCol_ColorButton_Border,       mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
			MV_CREATE_CONSTANT_TUPLE(mvThemeCol_ColorButton_BorderShadow, mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorButton_Rounding,        0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorButton_BorderSize,      0,  1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorButton_PopupRounding,   0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorButton_PopupBorderSize, 1,  1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorButton_PopupPaddingX,   8, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorButton_PopupPaddingY,   8, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvColorButton(const std::string& name, const mvColor& color);
		mvColorButton(const std::string& name, const mvColorButtonConfig& config);

		void draw() override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

		// cpp interface
		void updateConfig(mvAppItemConfig* config) override;
		mvAppItemConfig* getConfig() override;

	private:

		ImGuiColorEditFlags m_flags = ImGuiColorEditFlags_None;
		ImVec4 m_color;
		mvColorButtonConfig m_config;

	};

}
