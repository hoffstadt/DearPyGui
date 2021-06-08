#pragma once

#include "mvApp.h"
#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvInputText, MV_ITEM_DESC_DEFAULT, StorageValueTypes::String, 1);
	class mvInputText : public mvStringPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvInputText, add_input_text)

		MV_CREATE_CONSTANT(mvThemeCol_InputText_Text, ImGuiCol_Text, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_InputText_TextHighlight, ImGuiCol_TextSelectedBg,	0L);
		MV_CREATE_CONSTANT(mvThemeCol_InputText_Bg, ImGuiCol_FrameBg, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_InputText_Hint, ImGuiCol_TextDisabled, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_InputText_Border, ImGuiCol_Border, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_InputText_BorderShadow,ImGuiCol_BorderShadow, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_InputText_Rounding, ImGuiStyleVar_FrameRounding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_InputText_BorderSize, ImGuiStyleVar_FrameBorderSize, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_InputText_PaddingX, ImGuiStyleVar_FramePadding,	0L);
		MV_CREATE_CONSTANT(mvThemeStyle_InputText_PaddingY, ImGuiStyleVar_FramePadding,	1L);
		MV_CREATE_CONSTANT(mvThemeStyle_InputText_InnerSpacingX, ImGuiStyleVar_ItemInnerSpacing, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_InputText_InnerSpacingY, ImGuiStyleVar_ItemInnerSpacing, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_ADD_CONSTANT(mvThemeCol_InputText_Text, mvImGuiCol_Text, mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_InputText_TextHighlight,	mvImGuiCol_TextSelectedBg, mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_InputText_Bg, mvImGuiCol_FrameBg, mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_InputText_Hint, mvImGuiCol_TextDisabled, mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_InputText_Border, mvImGuiCol_Border, mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_InputText_BorderShadow,	mvImGuiCol_BorderShadow, mvColor(128, 128, 128, 63)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT_F(mvThemeStyle_InputText_Rounding, 0, 12),
			MV_ADD_CONSTANT_F(mvThemeStyle_InputText_BorderSize, 0,  1),
			MV_ADD_CONSTANT_F(mvThemeStyle_InputText_PaddingX, 4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_InputText_PaddingY, 3, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_InputText_InnerSpacingX, 4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_InputText_InnerSpacingY, 4, 20),
		MV_END_STYLE_CONSTANTS

	public:

		mvInputText(mvUUID uuid);

		void setEnabled        (bool value)     override;
		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		std::string         m_hint;
		bool                m_multiline = false;
		ImGuiInputTextFlags m_flags = 0;
		ImGuiInputTextFlags m_stor_flags = 0;

	};

}