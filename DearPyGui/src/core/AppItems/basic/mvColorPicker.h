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
	// mvColorPicker3
	//-----------------------------------------------------------------------------
	MV_REGISTER_WIDGET(mvColorPicker3, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Color, 1);
	class mvColorPicker3 : public mvColorPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvColorPicker3, add_color_picker3)

		MV_CREATE_CONSTANT(mvThemeCol_ColorPicker3_Text			, ImGuiCol_Text				, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorPicker3_Bg			, ImGuiCol_FrameBg			, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorPicker3_BgHovered	, ImGuiCol_FrameBgHovered	, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorPicker3_BgActive		, ImGuiCol_FrameBgActive	, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorPicker3_DragDrop		, ImGuiCol_DragDropTarget	, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorPicker3_Border		, ImGuiCol_Border			, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorPicker3_BorderShadow	, ImGuiCol_BorderShadow		, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_ColorPicker3_Rounding		, ImGuiStyleVar_FrameRounding	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorPicker3_BorderSize		, ImGuiStyleVar_FrameBorderSize	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorPicker3_PopupRounding	, ImGuiStyleVar_PopupRounding	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorPicker3_PopupBorderSize, ImGuiStyleVar_PopupBorderSize	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorPicker3_InnerSpacingX	, ImGuiStyleVar_ItemInnerSpacing, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorPicker3_InnerSpacingY	, ImGuiStyleVar_ItemInnerSpacing, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorPicker3_PopupPaddingX	, ImGuiStyleVar_WindowPadding	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorPicker3_PopupPaddingY	, ImGuiStyleVar_WindowPadding	, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorPicker3_PaddingX		, ImGuiStyleVar_FramePadding	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorPicker3_PaddingY		, ImGuiStyleVar_FramePadding	, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_ADD_CONSTANT(mvThemeCol_ColorPicker3_Text		, mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorPicker3_Bg			, mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorPicker3_BgHovered	, mvColor( 66, 150, 250, 102), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorPicker3_BgActive	, mvColor( 66, 150, 250, 171), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorPicker3_DragDrop	, mvColor(255, 255,   0, 230), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorPicker3_Border		, mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorPicker3_BorderShadow, mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT(mvThemeStyle_ColorPicker3_Rounding			, 0, 12),
			MV_ADD_CONSTANT(mvThemeStyle_ColorPicker3_BorderSize		, 0,  1),
			MV_ADD_CONSTANT(mvThemeStyle_ColorPicker3_PopupRounding		, 0, 12),
			MV_ADD_CONSTANT(mvThemeStyle_ColorPicker3_PopupBorderSize	, 1,  1),
			MV_ADD_CONSTANT(mvThemeStyle_ColorPicker3_InnerSpacingX		, 4, 20),
			MV_ADD_CONSTANT(mvThemeStyle_ColorPicker3_InnerSpacingY		, 4, 20),
			MV_ADD_CONSTANT(mvThemeStyle_ColorPicker3_PopupPaddingX		, 8, 20),
			MV_ADD_CONSTANT(mvThemeStyle_ColorPicker3_PopupPaddingY		, 8, 20),
			MV_ADD_CONSTANT(mvThemeStyle_ColorPicker3_PaddingX			, 4, 20),
			MV_ADD_CONSTANT(mvThemeStyle_ColorPicker3_PaddingY			, 3, 20),
		MV_END_STYLE_CONSTANTS

	public:

		mvColorPicker3(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y)               override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		ImGuiColorEditFlags m_flags = ImGuiColorEditFlags_None;

		std::array<float, 4> m_default_value = { 0.0f, 0.0f, 0.0f, 1.0f };
		bool                 m_no_alpha = false;
		bool                 m_no_small_preview = false;
		bool                 m_no_inputs = false;
		bool                 m_no_tooltip = false;
		bool                 m_no_label = false;
		bool                 m_no_side_preview = false;
		bool                 m_alpha_bar = false;
		bool                 m_alpha_preview = false;
		bool                 m_alpha_preview_half = false;
		bool                 m_display_rgb = false;
		bool                 m_display_hsv = false;
		bool                 m_display_hex = false;
		bool                 m_uint8 = false;
		bool                 m_floats = false;
		bool                 m_picker_hue_bar = false;
		bool                 m_picker_hue_wheel = false;
		bool                 m_input_rgb = false;
		bool                 m_input_hsv = false;

	};

	//-----------------------------------------------------------------------------
	// mvColorPicker4
	//-----------------------------------------------------------------------------
	MV_REGISTER_WIDGET(mvColorPicker4, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Color, 1);
	class mvColorPicker4 : public mvColorPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvColorPicker4, add_color_picker4)

		MV_CREATE_CONSTANT(mvThemeCol_ColorPicker4_Text			, ImGuiCol_Text				, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorPicker4_Bg			, ImGuiCol_FrameBg			, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorPicker4_BgHovered	, ImGuiCol_FrameBgHovered	, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorPicker4_BgActive		, ImGuiCol_FrameBgActive	, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorPicker4_DragDrop		, ImGuiCol_DragDropTarget	, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorPicker4_Border		, ImGuiCol_Border			, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorPicker4_BorderShadow	, ImGuiCol_BorderShadow		, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_ColorPicker4_Rounding		, ImGuiStyleVar_FrameRounding	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorPicker4_BorderSize		, ImGuiStyleVar_FrameBorderSize	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorPicker4_PopupRounding	, ImGuiStyleVar_PopupRounding	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorPicker4_PopupBorderSize, ImGuiStyleVar_PopupBorderSize	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorPicker4_InnerSpacingX	, ImGuiStyleVar_ItemInnerSpacing, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorPicker4_InnerSpacingY	, ImGuiStyleVar_ItemInnerSpacing, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorPicker4_PopupPaddingX	, ImGuiStyleVar_WindowPadding	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorPicker4_PopupPaddingY	, ImGuiStyleVar_WindowPadding	, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorPicker4_PaddingX		, ImGuiStyleVar_FramePadding	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorPicker4_PaddingY		, ImGuiStyleVar_FramePadding	, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_ADD_CONSTANT(mvThemeCol_ColorPicker4_Text		, mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorPicker4_Bg			, mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorPicker4_BgHovered	, mvColor( 66, 150, 250, 102), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorPicker4_BgActive	, mvColor( 66, 150, 250, 171), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorPicker4_DragDrop	, mvColor(255, 255,   0, 230), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorPicker4_Border		, mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorPicker4_BorderShadow, mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT(mvThemeStyle_ColorPicker4_Rounding			, 0, 12),
			MV_ADD_CONSTANT(mvThemeStyle_ColorPicker4_BorderSize		, 0,  1),
			MV_ADD_CONSTANT(mvThemeStyle_ColorPicker4_PopupRounding		, 0, 12),
			MV_ADD_CONSTANT(mvThemeStyle_ColorPicker4_PopupBorderSize	, 1,  1),
			MV_ADD_CONSTANT(mvThemeStyle_ColorPicker4_InnerSpacingX		, 4, 20),
			MV_ADD_CONSTANT(mvThemeStyle_ColorPicker4_InnerSpacingY		, 4, 20),
			MV_ADD_CONSTANT(mvThemeStyle_ColorPicker4_PopupPaddingX		, 8, 20),
			MV_ADD_CONSTANT(mvThemeStyle_ColorPicker4_PopupPaddingY		, 8, 20),
			MV_ADD_CONSTANT(mvThemeStyle_ColorPicker4_PaddingX			, 4, 20),
			MV_ADD_CONSTANT(mvThemeStyle_ColorPicker4_PaddingY			, 3, 20),
		MV_END_STYLE_CONSTANTS

	public:

		mvColorPicker4(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		ImGuiColorEditFlags m_flags = ImGuiColorEditFlags_None;

		std::array<float, 4> m_default_value = { 0.0f, 0.0f, 0.0f, 1.0f };
		bool                 m_no_alpha = false;
		bool                 m_no_small_preview = false;
		bool                 m_no_inputs = false;
		bool                 m_no_tooltip = false;
		bool                 m_no_label = false;
		bool                 m_no_side_preview = false;
		bool                 m_alpha_bar = false;
		bool                 m_alpha_preview = false;
		bool                 m_alpha_preview_half = false;
		bool                 m_display_rgb = false;
		bool                 m_display_hsv = false;
		bool                 m_display_hex = false;
		bool                 m_uint8 = false;
		bool                 m_floats = false;
		bool                 m_picker_hue_bar = false;
		bool                 m_picker_hue_wheel = false;
		bool                 m_input_rgb = false;
		bool                 m_input_hsv = false;

	};

}
