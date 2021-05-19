#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvColorPicker, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Color, 1);
	class mvColorPicker : public mvColorPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvColorPicker, add_color_picker)

		MV_CREATE_CONSTANT(mvThemeCol_ColorPicker_Text, ImGuiCol_Text, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorPicker_Bg, ImGuiCol_FrameBg, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorPicker_Hovered, ImGuiCol_FrameBgHovered, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorPicker_Active, ImGuiCol_FrameBgActive, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorPicker_DragDrop, ImGuiCol_DragDropTarget, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorPicker_Border, ImGuiCol_Border, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_ColorPicker_BorderShadow, ImGuiCol_BorderShadow, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_ColorPicker_Rounding, ImGuiStyleVar_FrameRounding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorPicker_BorderSize, ImGuiStyleVar_FrameBorderSize, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorPicker_PopupRounding, ImGuiStyleVar_PopupRounding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorPicker_PopupBorderSize, ImGuiStyleVar_PopupBorderSize, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorPicker_InnerSpacingX, ImGuiStyleVar_ItemInnerSpacing, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorPicker_InnerSpacingY, ImGuiStyleVar_ItemInnerSpacing, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorPicker_PopupPaddingX, ImGuiStyleVar_WindowPadding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorPicker_PopupPaddingY, ImGuiStyleVar_WindowPadding, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorPicker_PaddingX, ImGuiStyleVar_FramePadding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_ColorPicker_PaddingY, ImGuiStyleVar_FramePadding, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_ADD_CONSTANT(mvThemeCol_ColorPicker_Text, mvImGuiCol_Text, mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorPicker_Bg, mvImGuiCol_FrameBg, mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorPicker_Hovered, mvImGuiCol_FrameBgHovered, mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorPicker_Active, mvImGuiCol_FrameBgActive, mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorPicker_DragDrop, mvImGuiCol_DragDropTarget, mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorPicker_Border, mvImGuiCol_Border, mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_ColorPicker_BorderShadow, mvImGuiCol_BorderShadow, mvColor(128, 128, 128, 63)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT_F(mvThemeStyle_ColorPicker_Rounding			, 0, 12),
			MV_ADD_CONSTANT_F(mvThemeStyle_ColorPicker_BorderSize		, 0,  1),
			MV_ADD_CONSTANT_F(mvThemeStyle_ColorPicker_PopupRounding		, 0, 12),
			MV_ADD_CONSTANT_F(mvThemeStyle_ColorPicker_PopupBorderSize	, 1,  1),
			MV_ADD_CONSTANT_F(mvThemeStyle_ColorPicker_InnerSpacingX		, 4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_ColorPicker_InnerSpacingY		, 4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_ColorPicker_PopupPaddingX		, 8, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_ColorPicker_PopupPaddingY		, 8, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_ColorPicker_PaddingX			, 4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_ColorPicker_PaddingY			, 3, 20),
		MV_END_STYLE_CONSTANTS

	public:

		mvColorPicker(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificPositionalArgs(PyObject* dict) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		ImGuiColorEditFlags m_flags = ImGuiColorEditFlags__OptionsDefault;

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
		bool                 m_3component = false;

	};

}
