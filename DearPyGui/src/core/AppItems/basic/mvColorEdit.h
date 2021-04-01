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
	// mvColorEdit3
	//-----------------------------------------------------------------------------
	MV_REGISTER_WIDGET(mvColorEdit3);
	class mvColorEdit3 : public mvColorPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::mvColorEdit3, add_color_edit3)

		MV_CREATE_CONSTANT(mvThemeCol_ColorEdit3_Text					,  0L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorEdit3_TextHighlight		, 44L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorEdit3_Bg					,  7L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorEdit3_BgHovered			,  8L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorEdit3_BgActive				,  9L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorEdit3_PopupBg				,  4L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorEdit3_Border				,  5L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorEdit3_BorderShadow			,  6L, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit3_Rounding			, 11L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit3_BorderSize			, 12L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit3_PopupRounding		,  8L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit3_PopupBorderSize	,  9L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit3_InnerSpacingX		, 14L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit3_InnerSpacingY		, 14L, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit3_PopupPaddingX		,  1L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit3_PopupPaddingY		,  1L, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit3_PaddingX			, 10L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit3_PaddingY			, 10L, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_ADD_CONSTANT(mvThemeCol_ColorEdit3_Text,          mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorEdit3_TextHighlight, mvColor( 66, 150, 250,  89), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorEdit3_Bg,            mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorEdit3_BgHovered,     mvColor( 66, 150, 250, 102), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorEdit3_BgActive,      mvColor( 66, 150, 250, 171), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorEdit3_PopupBg,       mvColor( 20,  20,  20, 240), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorEdit3_Border,        mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorEdit3_BorderShadow,  mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit3_Rounding			, 0, 12),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit3_BorderSize			, 0,  1),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit3_PopupRounding		, 0, 12),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit3_PopupBorderSize	, 1,  1),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit3_InnerSpacingX		, 4, 20),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit3_InnerSpacingY		, 4, 20),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit3_PopupPaddingX		, 8, 20),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit3_PopupPaddingY		, 8, 20),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit3_PaddingX			, 4, 20),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit3_PaddingY			, 3, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvColorEdit3(const std::string& name, float* color, const std::string& dataSource);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
	
	private:

		ImGuiColorEditFlags m_flags = ImGuiColorEditFlags_None;

		std::array<float, 4> m_default_value = { 0.0f, 0.0f, 0.0f, 1.0f };
		bool                 m_no_alpha = false;
		bool                 m_no_picker = false;
		bool                 m_no_options = false;
		bool                 m_no_small_preview = false;
		bool                 m_no_inputs = false;
		bool                 m_no_tooltip = false;
		bool                 m_no_label = false;
		bool                 m_no_drag_drop = false;
		bool                 m_alpha_bar = false;
		bool                 m_alpha_preview = false;
		bool                 m_alpha_preview_half = false;
		bool                 m_display_rgb = false;
		bool                 m_display_hsv = false;
		bool                 m_display_hex = false;
		bool                 m_uint8 = false;
		bool                 m_floats = false;
		bool                 m_input_rgb = false;
		bool                 m_input_hsv = false;
	};

	//-----------------------------------------------------------------------------
	// mvColorEdit4
	//-----------------------------------------------------------------------------
	MV_REGISTER_WIDGET(mvColorEdit4);
	class mvColorEdit4 : public mvColorPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::mvColorEdit4, add_color_edit4)

		MV_CREATE_CONSTANT(mvThemeCol_ColorEdit4_Text					,  0L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorEdit4_TextHighlight		, 44L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorEdit4_Bg					,  7L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorEdit4_BgHovered			,  8L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorEdit4_BgActive				,  9L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorEdit4_PopupBg				,  4L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorEdit4_Border				,  5L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorEdit4_BorderShadow			,  6L, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit4_Rounding			, 11L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit4_BorderSize			, 12L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit4_PopupRounding		,  8L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit4_PopupBorderSize	,  9L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit4_InnerSpacingX		, 14L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit4_InnerSpacingY		, 14L, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit4_PopupPaddingX		,  1L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit4_PopupPaddingY		,  1L, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit4_PaddingX			, 10L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit4_PaddingY			, 10L, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_ADD_CONSTANT(mvThemeCol_ColorEdit4_Text,          mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorEdit4_TextHighlight, mvColor( 66, 150, 250,  89), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorEdit4_Bg,            mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorEdit4_BgHovered,     mvColor( 66, 150, 250, 102), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorEdit4_BgActive,      mvColor( 66, 150, 250, 171), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorEdit4_PopupBg,       mvColor( 20,  20,  20, 240), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorEdit4_Border,        mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorEdit4_BorderShadow,  mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit4_Rounding			, 0, 12),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit4_BorderSize			, 0,  1),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit4_PopupRounding		, 0, 12),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit4_PopupBorderSize	, 1,  1),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit4_InnerSpacingX		, 4, 20),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit4_InnerSpacingY		, 4, 20),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit4_PopupPaddingX		, 8, 20),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit4_PopupPaddingY		, 8, 20),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit4_PaddingX			, 4, 20),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit4_PaddingY			, 3, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvColorEdit4(const std::string& name, float* color, const std::string& dataSource);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		ImGuiColorEditFlags m_flags = ImGuiColorEditFlags_None;


		std::array<float, 4> m_default_value = { 0.0f, 0.0f, 0.0f, 1.0f };
		bool                 m_no_alpha = false;
		bool                 m_no_picker = false;
		bool                 m_no_options = false;
		bool                 m_no_small_preview = false;
		bool                 m_no_inputs = false;
		bool                 m_no_tooltip = false;
		bool                 m_no_label = false;
		bool                 m_no_drag_drop = false;
		bool                 m_alpha_bar = false;
		bool                 m_alpha_preview = false;
		bool                 m_alpha_preview_half = false;
		bool                 m_display_rgb = false;
		bool                 m_display_hsv = false;
		bool                 m_display_hex = false;
		bool                 m_uint8 = false;
		bool                 m_floats = false;
		bool                 m_input_rgb = false;
		bool                 m_input_hsv = false;

	};

}
