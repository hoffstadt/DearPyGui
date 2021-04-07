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
	MV_REGISTER_WIDGET(mvColorEdit3, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Color, 1);
	class mvColorEdit3 : public mvColorPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPITEM_TYPE(mvAppItemType::mvColorEdit3, add_color_edit3)

		MV_CREATE_CONSTANT(mvThemeCol_ColorEdit3_Text				, ImGuiCol_Text				, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorEdit3_TextHighlight		, ImGuiCol_TextSelectedBg	, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorEdit3_Bg					, ImGuiCol_FrameBg			, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorEdit3_BgHovered			, ImGuiCol_FrameBgHovered	, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorEdit3_BgActive			, ImGuiCol_FrameBgActive	, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorEdit3_DragDrop			, ImGuiCol_DragDropTarget	, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorEdit3_PopupBg			, ImGuiCol_PopupBg			, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorEdit3_Border				, ImGuiCol_Border			, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorEdit3_BorderShadow		, ImGuiCol_BorderShadow		, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit3_Rounding			, ImGuiStyleVar_FrameRounding	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit3_BorderSize		, ImGuiStyleVar_FrameBorderSize	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit3_PopupRounding	, ImGuiStyleVar_PopupRounding	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit3_PopupBorderSize	, ImGuiStyleVar_PopupBorderSize	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit3_InnerSpacingX	, ImGuiStyleVar_ItemInnerSpacing, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit3_InnerSpacingY	, ImGuiStyleVar_ItemInnerSpacing, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit3_PopupPaddingX	, ImGuiStyleVar_WindowPadding	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit3_PopupPaddingY	, ImGuiStyleVar_WindowPadding	, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit3_PaddingX			, ImGuiStyleVar_FramePadding	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit3_PaddingY			, ImGuiStyleVar_FramePadding	, 1L);

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
			MV_ADD_CONSTANT(mvThemeCol_ColorEdit3_DragDrop,		 mvColor(255, 255,   0, 230), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorEdit3_PopupBg,       mvColor( 20,  20,  20, 240), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorEdit3_Border,        mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorEdit3_BorderShadow,  mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit3_Rounding			, 0, 12),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit3_BorderSize			, 0,  1),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit3_PopupRounding		, 0, 12),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit3_PopupBorderSize		, 1,  1),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit3_InnerSpacingX		, 4, 20),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit3_InnerSpacingY		, 4, 20),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit3_PopupPaddingX		, 8, 20),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit3_PopupPaddingY		, 8, 20),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit3_PaddingX			, 4, 20),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit3_PaddingY			, 3, 20),
		MV_END_STYLE_CONSTANTS

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
	MV_REGISTER_WIDGET(mvColorEdit4, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Color, 1);
	class mvColorEdit4 : public mvColorPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPITEM_TYPE(mvAppItemType::mvColorEdit4, add_color_edit4)

		MV_CREATE_CONSTANT(mvThemeCol_ColorEdit4_Text			, ImGuiCol_Text				, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorEdit4_TextHighlight	, ImGuiCol_TextSelectedBg	, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorEdit4_Bg				, ImGuiCol_FrameBg			, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorEdit4_BgHovered		, ImGuiCol_FrameBgHovered	, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorEdit4_BgActive		, ImGuiCol_FrameBgActive	, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorEdit4_DragDrop		, ImGuiCol_DragDropTarget	, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorEdit4_PopupBg		, ImGuiCol_PopupBg			, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorEdit4_Border			, ImGuiCol_Border			, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorEdit4_BorderShadow	, ImGuiCol_BorderShadow		, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit4_Rounding			, ImGuiStyleVar_FrameRounding	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit4_BorderSize		, ImGuiStyleVar_FrameBorderSize	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit4_PopupRounding	, ImGuiStyleVar_PopupRounding	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit4_PopupBorderSize	, ImGuiStyleVar_PopupBorderSize	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit4_InnerSpacingX	, ImGuiStyleVar_ItemInnerSpacing, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit4_InnerSpacingY	, ImGuiStyleVar_ItemInnerSpacing, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit4_PopupPaddingX	, ImGuiStyleVar_WindowPadding	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit4_PopupPaddingY	, ImGuiStyleVar_WindowPadding	, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit4_PaddingX			, ImGuiStyleVar_FramePadding	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorEdit4_PaddingY			, ImGuiStyleVar_FramePadding	, 1L);

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
			MV_ADD_CONSTANT(mvThemeCol_ColorEdit4_DragDrop,		 mvColor(255, 255,   0, 230), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorEdit4_PopupBg,       mvColor( 20,  20,  20, 240), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorEdit4_Border,        mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorEdit4_BorderShadow,  mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit4_Rounding			, 0, 12),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit4_BorderSize			, 0,  1),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit4_PopupRounding		, 0, 12),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit4_PopupBorderSize		, 1,  1),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit4_InnerSpacingX		, 4, 20),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit4_InnerSpacingY		, 4, 20),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit4_PopupPaddingX		, 8, 20),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit4_PopupPaddingY		, 8, 20),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit4_PaddingX			, 4, 20),
			MV_ADD_CONSTANT(mvThemeStyle_ColorEdit4_PaddingY			, 3, 20),
		MV_END_STYLE_CONSTANTS

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
