#pragma once

#include "mvTypeBases.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvColorPicker3
//     * mvColorPicker4
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvColorPicker Config Struct
	//-----------------------------------------------------------------------------
	struct mvColorPickerConfig : public mvAppItemConfig
	{
		std::array<float, 4> default_value = {0.0f, 0.0f, 0.0f, 1.0f};
		bool                 no_alpha           = false;
		bool                 no_small_preview   = false;
		bool                 no_inputs          = false;
		bool                 no_tooltip         = false;
		bool                 no_label           = false;
		bool                 no_side_preview    = false;
		bool                 alpha_bar          = false;
		bool                 alpha_preview      = false;
		bool                 alpha_preview_half = false;
		bool                 display_rgb        = false;
		bool                 display_hsv        = false;
		bool                 display_hex        = false;
		bool                 uint8              = false;
		bool                 floats             = false;
		bool                 picker_hue_bar     = false;
		bool                 picker_hue_wheel   = false;
		bool                 input_rgb          = false;
		bool                 input_hsv          = false;
	};

#ifdef MV_CPP
	void add_color_picker3(const std::string& name, const mvColorPickerConfig& config = {});
	void add_color_picker4(const std::string& name, const mvColorPickerConfig& config = {});
#else
	PyObject* add_color_picker3(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_color_picker4(PyObject* self, PyObject* args, PyObject* kwargs);
#endif

	//-----------------------------------------------------------------------------
	// mvColorPicker3
	//-----------------------------------------------------------------------------
	class mvColorPicker3 : public mvColorPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::ColorPicker3, mvColorPicker3, "add_color_picker3")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeCol_ColorPicker3_Text, 0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeCol_ColorPicker3_Bg, 7L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeCol_ColorPicker3_BgHovered, 8L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeCol_ColorPicker3_BgActive, 9L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeCol_ColorPicker3_Border, 5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeCol_ColorPicker3_BorderShadow, 6L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeStyle_ColorPicker3_Rounding, 11L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeStyle_ColorPicker3_BorderSize, 12L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeStyle_ColorPicker3_PopupRounding, 8L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeStyle_ColorPicker3_PopupBorderSize, 9L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeStyle_ColorPicker3_InnerSpacingX, 14L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeStyle_ColorPicker3_InnerSpacingY, 14L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeStyle_ColorPicker3_PopupPaddingX, 1L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeStyle_ColorPicker3_PopupPaddingY, 1L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeStyle_ColorPicker3_PaddingX, 10L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker3, mvThemeStyle_ColorPicker3_PaddingY, 10L, 1L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker3_Text,         mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker3_Bg,           mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker3_BgHovered,    mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker3_BgActive,     mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker3_Border,       mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker3_BorderShadow, mvColor(255, 255, 255, 255)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker3_Rounding, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker3_BorderSize, 0, 1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker3_PopupRounding, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker3_PopupBorderSize, 0, 1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker3_InnerSpacingX, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker3_InnerSpacingY, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker3_PopupPaddingX, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker3_PopupPaddingY, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker3_PaddingX, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker3_PaddingY, 0, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvColorPicker3(const std::string& name, float* color, const std::string& dataSource);
		mvColorPicker3(const std::string& name, const mvColorPickerConfig& config);

		void draw()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

		// cpp interface
		void updateConfig(mvAppItemConfig* config) override;
		mvAppItemConfig* getConfig() override;

	private:

		ImGuiColorEditFlags m_flags = ImGuiColorEditFlags_None;
		mvColorPickerConfig m_config;

	};

	//-----------------------------------------------------------------------------
	// mvColorPicker4
	//-----------------------------------------------------------------------------
	class mvColorPicker4 : public mvColorPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::ColorPicker4, mvColorPicker4, "add_color_picker4")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeCol_ColorPicker4_Text, 0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeCol_ColorPicker4_Bg, 7L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeCol_ColorPicker4_BgHovered, 8L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeCol_ColorPicker4_BgActive, 9L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeCol_ColorPicker4_Border, 5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeCol_ColorPicker4_BorderShadow, 6L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeStyle_ColorPicker4_Rounding, 11L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeStyle_ColorPicker4_BorderSize, 12L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeStyle_ColorPicker4_PopupRounding, 8L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeStyle_ColorPicker4_PopupBorderSize, 9L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeStyle_ColorPicker4_InnerSpacingX, 14L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeStyle_ColorPicker4_InnerSpacingY, 14L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeStyle_ColorPicker4_PopupPaddingX, 1L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeStyle_ColorPicker4_PopupPaddingY, 1L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeStyle_ColorPicker4_PaddingX, 10L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorPicker4, mvThemeStyle_ColorPicker4_PaddingY, 10L, 1L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker4_Text,         mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker4_Bg,           mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker4_BgHovered,    mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker4_BgActive,     mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker4_Border,       mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorPicker4_BorderShadow, mvColor(255, 255, 255, 255)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker4_Rounding, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker4_BorderSize, 0, 1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker4_PopupRounding, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker4_PopupBorderSize, 0, 1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker4_InnerSpacingX, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker4_InnerSpacingY, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker4_PopupPaddingX, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker4_PopupPaddingY, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker4_PaddingX, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorPicker4_PaddingY, 0, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvColorPicker4(const std::string& name, float* color, const std::string& dataSource);
		mvColorPicker4(const std::string& name, const mvColorPickerConfig& config);

		void draw()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

		// cpp interface
		void updateConfig(mvAppItemConfig* config) override;
		mvAppItemConfig* getConfig() override;

	private:

		ImGuiColorEditFlags m_flags = ImGuiColorEditFlags_None;
		mvColorPickerConfig m_config;

	};

}
