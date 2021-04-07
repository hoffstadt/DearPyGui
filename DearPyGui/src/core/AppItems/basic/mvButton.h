#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvButton, MV_ITEM_DESC_DEFAULT, StorageValueTypes::None, 1);
	class mvButton : public mvAppItem
	{
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION2(mvAppItemType::mvButton, add_button)

		MV_CREATE_CONSTANT(mvDir_None	, -1L, 0L);
		MV_CREATE_CONSTANT(mvDir_Left	,  0L, 0L);
		MV_CREATE_CONSTANT(mvDir_Right	,  1L, 0L);
		MV_CREATE_CONSTANT(mvDir_Up		,  2L, 0L);
		MV_CREATE_CONSTANT(mvDir_Down	,  3L, 0L);

		MV_CREATE_CONSTANT(mvThemeCol_Button_Text			, ImGuiCol_Text			, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Button_Bg				, ImGuiCol_Button		, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Button_Hovered		, ImGuiCol_ButtonHovered, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Button_Active			, ImGuiCol_ButtonActive	, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Button_Border			, ImGuiCol_Border		, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Button_BorderShadow	, ImGuiCol_BorderShadow	, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_Button_Rounding		, ImGuiStyleVar_FrameRounding	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Button_BorderSize	, ImGuiStyleVar_FrameBorderSize	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Button_TextAlignX	, ImGuiStyleVar_ButtonTextAlign	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Button_TextAlignY	, ImGuiStyleVar_ButtonTextAlign	, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Button_PaddingX		, ImGuiStyleVar_FramePadding	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Button_PaddingY		, ImGuiStyleVar_FramePadding	, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
			MV_ADD_GENERAL_CONSTANT(mvDir_None),
			MV_ADD_GENERAL_CONSTANT(mvDir_Left),
			MV_ADD_GENERAL_CONSTANT(mvDir_Right),
			MV_ADD_GENERAL_CONSTANT(mvDir_Up),
			MV_ADD_GENERAL_CONSTANT(mvDir_Down),
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_ADD_CONSTANT(mvThemeCol_Button_Text,         mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Button_Bg,           mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Button_Hovered,      mvColor( 66, 150, 250, 102), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Button_Active,       mvColor( 66, 150, 250, 171), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Button_Border,       mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Button_BorderShadow, mvColor(  0,   0,   0,   0), mvColor(128, 128, 128, 63)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT(mvThemeStyle_Button_Rounding	, 0    ,12),
			MV_ADD_CONSTANT(mvThemeStyle_Button_BorderSize	, 0    , 1),
			MV_ADD_CONSTANT(mvThemeStyle_Button_TextAlignX	, 0.5f , 1),
			MV_ADD_CONSTANT(mvThemeStyle_Button_TextAlignY	, 0.5f , 1),
			MV_ADD_CONSTANT(mvThemeStyle_Button_PaddingX	, 4    ,20),
			MV_ADD_CONSTANT(mvThemeStyle_Button_PaddingY	, 3    ,20),
		MV_END_STYLE_CONSTANTS

	public:

		mvButton(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		ImGuiDir m_direction = ImGuiDir_Up;
		bool     m_small_button = false;
		bool     m_arrow = false;

	};

}
