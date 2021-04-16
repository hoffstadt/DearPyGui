#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvCheckbox, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Bool, 1);
	class mvCheckbox : public mvBoolPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvCheckbox, add_checkbox)

		MV_CREATE_CONSTANT(mvThemeCol_CheckBox_Text, ImGuiCol_Text				, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_CheckBox_Bg, ImGuiCol_FrameBg			, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_CheckBox_Hovered, ImGuiCol_FrameBgHovered	, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_CheckBox_Active, ImGuiCol_FrameBgActive	, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_CheckBox_Marker, ImGuiCol_CheckMark		, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_CheckBox_Border, ImGuiCol_Border			, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_CheckBox_BorderShadow, ImGuiCol_BorderShadow		, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_Checkbox_Rounding, ImGuiStyleVar_FrameRounding	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Checkbox_BorderSize, ImGuiStyleVar_FrameBorderSize	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Checkbox_PaddingX, ImGuiStyleVar_FramePadding	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Checkbox_PaddingY, ImGuiStyleVar_FramePadding	, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Checkbox_InnerItemSpacingX, ImGuiStyleVar_ItemInnerSpacing, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Checkbox_InnerItemSpacingY, ImGuiStyleVar_ItemInnerSpacing, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_ADD_CONSTANT(mvThemeCol_CheckBox_Text, mvImGuiCol_Text, mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_CheckBox_Bg, mvImGuiCol_FrameBg,  mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_CheckBox_Hovered, mvImGuiCol_FrameBgHovered, mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_CheckBox_Active, mvImGuiCol_FrameBgActive, mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_CheckBox_Marker, mvImGuiCol_CheckMark, mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_CheckBox_Border, mvImGuiCol_Border, mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_CheckBox_BorderShadow, mvImGuiCol_BorderShadow, mvColor(128, 128, 128, 63)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT_F(mvThemeStyle_Checkbox_BorderSize			, 0,  1),
			MV_ADD_CONSTANT_F(mvThemeStyle_Checkbox_Rounding			, 0, 12),
			MV_ADD_CONSTANT_F(mvThemeStyle_Checkbox_PaddingX			, 4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Checkbox_PaddingY			, 3, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Checkbox_InnerItemSpacingX	, 4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_Checkbox_InnerItemSpacingY	, 4, 20),
		MV_END_STYLE_CONSTANTS

	public:

		mvCheckbox(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y) override;

	private:

		bool m_default_value = false;
		std::string m_source = "";

	};

}
