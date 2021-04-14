#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvRadioButton, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Int, 1);
	class mvRadioButton : public mvIntPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvRadioButton, add_radio_button)

		MV_CREATE_CONSTANT(mvThemeCol_RadioButton_Text			, ImGuiCol_Text				, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_RadioButton_Bg			, ImGuiCol_FrameBg			, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_RadioButton_BgHovered		, ImGuiCol_FrameBgHovered	, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_RadioButton_BgActive		, ImGuiCol_CheckMark		, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_RadioButton_Border		, ImGuiCol_Border			, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_RadioButton_BorderShadow	, ImGuiCol_BorderShadow		, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_RadioButton_BorderSize		, ImGuiStyleVar_FrameBorderSize	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_RadioButton_PaddingX		, ImGuiStyleVar_FramePadding	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_RadioButton_PaddingY		, ImGuiStyleVar_FramePadding	, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_RadioButton_InnerSpacingX	, ImGuiStyleVar_ItemInnerSpacing, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_RadioButton_InnerSpacingY	, ImGuiStyleVar_ItemInnerSpacing, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_ADD_CONSTANT(mvThemeCol_RadioButton_Text,            mvColor_TextBackground(255), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_RadioButton_Bg,				mvColor_Primary(138), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_RadioButton_BgHovered,		mvColor_Primary(200), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_RadioButton_BgActive,		mvColor_Primary(255), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_RadioButton_Border,			mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_RadioButton_BorderShadow,	mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT_F(mvThemeStyle_RadioButton_BorderSize,    0,  1),
			MV_ADD_CONSTANT_F(mvThemeStyle_RadioButton_PaddingX,      4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_RadioButton_PaddingY,      3, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_RadioButton_InnerSpacingX, 4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_RadioButton_InnerSpacingY, 4, 20),
		MV_END_STYLE_CONSTANTS

	public:

		mvRadioButton(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificPositionalArgs(PyObject* dict) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		std::vector<std::string> m_itemnames;
		bool                     m_horizontal = false;
	};

}
