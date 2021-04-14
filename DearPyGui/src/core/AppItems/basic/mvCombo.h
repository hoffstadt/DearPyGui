#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvCombo, MV_ITEM_DESC_DEFAULT, StorageValueTypes::String, 1);
	class mvCombo : public mvStringPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvCombo, add_combo)

		MV_CREATE_CONSTANT(mvThemeCol_Combo_Text				, ImGuiCol_Text					, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Combo_Selected			, ImGuiCol_Header				, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Combo_Hovered				, ImGuiCol_HeaderHovered		, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Combo_Active				, ImGuiCol_HeaderActive			, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Combo_Bg					, ImGuiCol_FrameBg				, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Combo_BgHovered			, ImGuiCol_FrameBgHovered		, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Combo_DropBg				, ImGuiCol_PopupBg				, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Combo_DropButtonBg		, ImGuiCol_Button				, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Combo_DropButtonHovered	, ImGuiCol_ButtonHovered		, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Combo_Scrollbar			, ImGuiCol_ScrollbarBg			, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Combo_ScrollbarGrab		, ImGuiCol_ScrollbarGrab		, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Combo_ScrollbarGrabHovered, ImGuiCol_ScrollbarGrabHovered	, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Combo_ScrollbarGrabActive	, ImGuiCol_ScrollbarGrabActive	, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Combo_Border				, ImGuiCol_Border				, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Combo_BorderShadow		, ImGuiCol_BorderShadow			, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_Combo_BorderSize		, ImGuiStyleVar_FrameBorderSize		, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Combo_Rounding			, ImGuiStyleVar_FrameRounding		, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Combo_PaddingX			, ImGuiStyleVar_FramePadding		, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Combo_PaddingY			, ImGuiStyleVar_FramePadding		, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Combo_DropPaddingX		, ImGuiStyleVar_WindowPadding		, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Combo_DropPaddingY		, ImGuiStyleVar_WindowPadding		, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Combo_DropBorderSize	, ImGuiStyleVar_PopupBorderSize		, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Combo_DropRounding		, ImGuiStyleVar_PopupRounding		, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Combo_ItemSpacingX		, ImGuiStyleVar_ItemSpacing			, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Combo_ItemSpacingY		, ImGuiStyleVar_ItemSpacing			, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Combo_ScrollbarSize		, ImGuiStyleVar_ScrollbarSize		, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Combo_ScrollbarRounding	, ImGuiStyleVar_ScrollbarRounding	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Combo_TextAlignX		, ImGuiStyleVar_SelectableTextAlign	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Combo_TextAlignY		, ImGuiStyleVar_SelectableTextAlign	, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_ADD_CONSTANT(mvThemeCol_Combo_Text,					mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Combo_Selected,				mvColor( 66, 150, 250, 171), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Combo_Hovered,				mvColor( 41,  74,  74, 138), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Combo_Active,				mvColor( 66, 150, 250, 171), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Combo_Bg,					mvColor_Primary(80), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Combo_BgHovered,				mvColor( 41,  74,  74, 138), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Combo_DropBg,				mvColor( 20,  20,  20, 240), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Combo_DropButtonBg,          mvColor_Primary(138), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Combo_DropButtonHovered,     mvColor_Primary(200), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Combo_Scrollbar,             mvColor_Background(250), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Combo_ScrollbarGrab,         mvColor_Surface(200), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Combo_ScrollbarGrabHovered,  mvColor_Surface(250), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Combo_ScrollbarGrabActive,   mvColor_Surface(255), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Combo_Border,				mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Combo_BorderShadow,			mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT_F(mvThemeStyle_Combo_BorderSize		, 0,  1),
			MV_ADD_CONSTANT_F(mvThemeStyle_Combo_Rounding			, 0, 12),
			MV_ADD_CONSTANT_F(mvThemeStyle_Combo_PaddingX			, 4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Combo_PaddingY			, 3, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Combo_DropPaddingX		, 8, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Combo_DropPaddingY		, 8, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Combo_DropBorderSize	, 1,  1),
			MV_ADD_CONSTANT_F(mvThemeStyle_Combo_DropRounding		, 0, 12),
			MV_ADD_CONSTANT_F(mvThemeStyle_Combo_ItemSpacingX		, 8, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Combo_ItemSpacingY		, 4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Combo_ScrollbarSize	,14, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Combo_ScrollbarRounding, 9, 12),
			MV_ADD_CONSTANT_F(mvThemeStyle_Combo_TextAlignX		, 0, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Combo_TextAlignY		, 0, 20),
		MV_END_STYLE_CONSTANTS

	public:

		mvCombo(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificPositionalArgs(PyObject* dict) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:
		ImGuiComboFlags m_flags = ImGuiComboFlags_None;

		std::vector<std::string> m_items;
		std::string              m_default_value = "";
		bool                     m_popup_align_left = false;
		bool                     m_height_small = false;
		bool                     m_height_regular = false;
		bool                     m_height_large = false;
		bool                     m_height_largest = false;
		bool                     m_no_arrow_button = false;
		bool                     m_no_preview = false;
	};

}
