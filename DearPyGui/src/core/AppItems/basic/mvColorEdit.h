#pragma once

#include "mvTypeBases.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvColorEdit3
//     * mvColorEdit4
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvColorEdit Configuration Struct
	//-----------------------------------------------------------------------------
	struct mvColorEditConfig : public mvAppItemConfig
	{
		std::array<float, 4> default_value = {0.0f, 0.0f, 0.0f, 1.0f};
		bool                 no_alpha           = false;
		bool                 no_picker          = false;
		bool                 no_options         = false;
		bool                 no_small_preview   = false;
		bool                 no_inputs          = false;
		bool                 no_tooltip         = false;
		bool                 no_label           = false;
		bool                 no_drag_drop       = false;
		bool                 alpha_bar          = false;
		bool                 alpha_preview      = false;
		bool                 alpha_preview_half = false;
		bool                 display_rgb        = false;
		bool                 display_hsv        = false;
		bool                 display_hex        = false;
		bool                 uint8              = false;
		bool                 floats             = false;
		bool                 input_rgb          = false;
		bool                 input_hsv          = false;
	};

#ifdef MV_CPP
	void add_color_edit3(const std::string& name, const mvColorEditConfig& config = {});
	void add_color_edit4(const std::string& name, const mvColorEditConfig& config = {});
#else
	PyObject* add_color_edit3(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_color_edit4(PyObject* self, PyObject* args, PyObject* kwargs);
#endif

	class mvColorEdit3 : public mvColorPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::ColorEdit3, mvColorEdit3, "add_color_edit3")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeCol_ColorEdit3_Text					,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeCol_ColorEdit3_TextHighlight			, 44L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeCol_ColorEdit3_Bg					,  7L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeCol_ColorEdit3_BgHovered				,  8L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeCol_ColorEdit3_BgActive				,  9L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeCol_ColorEdit3_PopupBg				,  4L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeCol_ColorEdit3_Border				,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeCol_ColorEdit3_BorderShadow			,  6L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeStyle_ColorEdit3_Rounding			, 11L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeStyle_ColorEdit3_BorderSize			, 12L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeStyle_ColorEdit3_PopupRounding		,  8L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeStyle_ColorEdit3_PopupBorderSize		,  9L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeStyle_ColorEdit3_InnerSpacingX		, 14L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeStyle_ColorEdit3_InnerSpacingY		, 14L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeStyle_ColorEdit3_PopupPaddingX		,  1L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeStyle_ColorEdit3_PopupPaddingY		,  1L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeStyle_ColorEdit3_PaddingX			, 10L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit3, mvThemeStyle_ColorEdit3_PaddingY			, 10L, 1L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorEdit3_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorEdit3_TextHighlight),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorEdit3_Bg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorEdit3_BgHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorEdit3_BgActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorEdit3_PopupBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorEdit3_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorEdit3_BorderShadow),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit3_Rounding			, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit3_BorderSize			, 0,  1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit3_PopupRounding		, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit3_PopupBorderSize	, 0,  1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit3_InnerSpacingX		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit3_InnerSpacingY		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit3_PopupPaddingX		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit3_PopupPaddingY		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit3_PaddingX			, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit3_PaddingY			, 0, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvColorEdit3(const std::string& name, float* color, const std::string& dataSource);
		mvColorEdit3(const std::string& name, const mvColorEditConfig& config);

		void draw() override;

		// cpp interface
		void             updateConfig(mvAppItemConfig* config) override;
		mvAppItemConfig* getConfig() override;


#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP
	
	private:

		ImGuiColorEditFlags m_flags = ImGuiColorEditFlags_None;
		mvColorEditConfig   m_config;

	};

	//-----------------------------------------------------------------------------
	// mvColorEdit4
	//-----------------------------------------------------------------------------
	class mvColorEdit4 : public mvColorPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::ColorEdit4, mvColorEdit4, "add_color_edit4")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeCol_ColorEdit4_Text					,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeCol_ColorEdit4_TextHighlight			, 44L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeCol_ColorEdit4_Bg					,  7L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeCol_ColorEdit4_BgHovered				,  8L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeCol_ColorEdit4_BgActive				,  9L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeCol_ColorEdit4_PopupBg				,  4L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeCol_ColorEdit4_Border				,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeCol_ColorEdit4_BorderShadow			,  6L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeStyle_ColorEdit4_Rounding			, 11L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeStyle_ColorEdit4_BorderSize			, 12L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeStyle_ColorEdit4_PopupRounding		,  8L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeStyle_ColorEdit4_PopupBorderSize		,  9L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeStyle_ColorEdit4_InnerSpacingX		, 14L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeStyle_ColorEdit4_InnerSpacingY		, 14L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeStyle_ColorEdit4_PopupPaddingX		,  1L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeStyle_ColorEdit4_PopupPaddingY		,  1L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeStyle_ColorEdit4_PaddingX			, 10L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::ColorEdit4, mvThemeStyle_ColorEdit4_PaddingY			, 10L, 1L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorEdit4_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorEdit4_TextHighlight),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorEdit4_Bg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorEdit4_BgHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorEdit4_BgActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorEdit4_PopupBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorEdit4_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_ColorEdit4_BorderShadow),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit4_Rounding			, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit4_BorderSize			, 0,  1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit4_PopupRounding		, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit4_PopupBorderSize	, 0,  1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit4_InnerSpacingX		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit4_InnerSpacingY		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit4_PopupPaddingX		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit4_PopupPaddingY		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit4_PaddingX			, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_ColorEdit4_PaddingY			, 0, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvColorEdit4(const std::string& name, float* color, const std::string& dataSource);
		mvColorEdit4(const std::string& name, const mvColorEditConfig& config);

		void draw() override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

		// cpp interface
		void             updateConfig(mvAppItemConfig* config) override;
		mvAppItemConfig* getConfig() override;

	private:

		ImGuiColorEditFlags m_flags = ImGuiColorEditFlags_None;
		mvColorEditConfig   m_config;

	};

}
