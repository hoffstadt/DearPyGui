#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvCombo, MV_ITEM_DESC_DEFAULT, StorageValueTypes::String, 1);
	class mvCombo : public mvStringPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvCombo, add_combo)

		MV_CREATE_CONSTANT(mvThemeCol_Combo_Text, ImGuiCol_Text, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Combo_Bg, ImGuiCol_FrameBg, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Combo_Hovered, ImGuiCol_FrameBgHovered, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Combo_PopupBg, ImGuiCol_PopupBg, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Combo_PopupSelected, ImGuiCol_Header, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Combo_PopupHovered, ImGuiCol_HeaderHovered, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Combo_PopupActive, ImGuiCol_HeaderActive, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Combo_ButtonBg, ImGuiCol_Button, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Combo_ButtonHovered, ImGuiCol_ButtonHovered, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Combo_Scrollbar, ImGuiCol_ScrollbarBg, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Combo_ScrollbarGrab, ImGuiCol_ScrollbarGrab, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Combo_ScrollbarGrabHovered, ImGuiCol_ScrollbarGrabHovered, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Combo_ScrollbarGrabActive, ImGuiCol_ScrollbarGrabActive, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Combo_Border, ImGuiCol_Border, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Combo_BorderShadow, ImGuiCol_BorderShadow, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_Combo_BorderSize, ImGuiStyleVar_FrameBorderSize, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Combo_Rounding, ImGuiStyleVar_FrameRounding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Combo_PaddingX, ImGuiStyleVar_FramePadding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Combo_PaddingY, ImGuiStyleVar_FramePadding, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Combo_DropPaddingX, ImGuiStyleVar_WindowPadding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Combo_DropPaddingY, ImGuiStyleVar_WindowPadding, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Combo_DropBorderSize, ImGuiStyleVar_PopupBorderSize, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Combo_DropRounding, ImGuiStyleVar_PopupRounding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Combo_ItemSpacingX, ImGuiStyleVar_ItemSpacing, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Combo_ItemSpacingY, ImGuiStyleVar_ItemSpacing, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Combo_ScrollbarSize, ImGuiStyleVar_ScrollbarSize, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Combo_ScrollbarRounding, ImGuiStyleVar_ScrollbarRounding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Combo_TextAlignX, ImGuiStyleVar_SelectableTextAlign, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Combo_TextAlignY, ImGuiStyleVar_SelectableTextAlign, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_ADD_CONSTANT(mvThemeCol_Combo_Text, mvImGuiCol_Text, mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Combo_Bg, mvImGuiCol_FrameBg, mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Combo_Hovered, mvImGuiCol_FrameBgHovered, mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Combo_PopupBg, mvImGuiCol_PopupBg, mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Combo_PopupSelected, mvImGuiCol_Header, mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Combo_PopupHovered, mvImGuiCol_HeaderHovered, mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Combo_PopupActive, mvImGuiCol_HeaderActive, mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Combo_ButtonBg, mvImGuiCol_Button, mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Combo_ButtonHovered, mvImGuiCol_ButtonHovered, mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Combo_Scrollbar, mvImGuiCol_ScrollbarBg, mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Combo_ScrollbarGrab, mvImGuiCol_ScrollbarGrab, mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Combo_ScrollbarGrabHovered, mvImGuiCol_ScrollbarGrabHovered, mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Combo_ScrollbarGrabActive, mvImGuiCol_ScrollbarGrabActive, mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Combo_Border, mvImGuiCol_Border, mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Combo_BorderShadow, mvImGuiCol_BorderShadow, mvColor(128, 128, 128, 63)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT_F(mvThemeStyle_Combo_BorderSize, 0,  1),
			MV_ADD_CONSTANT_F(mvThemeStyle_Combo_Rounding, 0, 12),
			MV_ADD_CONSTANT_F(mvThemeStyle_Combo_PaddingX, 4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Combo_PaddingY, 3, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Combo_DropPaddingX, 8, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Combo_DropPaddingY, 8, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Combo_DropBorderSize	, 1,  1),
			MV_ADD_CONSTANT_F(mvThemeStyle_Combo_DropRounding, 0, 12),
			MV_ADD_CONSTANT_F(mvThemeStyle_Combo_ItemSpacingX, 8, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Combo_ItemSpacingY, 4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Combo_ScrollbarSize,14, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Combo_ScrollbarRounding, 0, 12),
			MV_ADD_CONSTANT_F(mvThemeStyle_Combo_TextAlignX, 0, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Combo_TextAlignY, 0, 20),
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
