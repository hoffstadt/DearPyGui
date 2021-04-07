#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvListbox, MV_ITEM_DESC_DEFAULT, StorageValueTypes::Int, 1);
	class mvListbox : public mvIntPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvListbox, add_listbox)

		MV_CREATE_CONSTANT(mvThemeCol_Listbox_Text                , ImGuiCol_Text					, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Listbox_Selected            , ImGuiCol_Header					, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Listbox_Hovered             , ImGuiCol_HeaderHovered			, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Listbox_Active              , ImGuiCol_HeaderActive			, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Listbox_Bg                  , ImGuiCol_FrameBg				, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Listbox_Border              , ImGuiCol_Border					, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Listbox_Scrollbar           , ImGuiCol_ScrollbarBg			, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Listbox_ScrollbarGrab       , ImGuiCol_ScrollbarGrab			, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Listbox_ScrollbarGrabHovered, ImGuiCol_ScrollbarGrabHovered	, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_Listbox_ScrollbarGrabActive , ImGuiCol_ScrollbarGrabActive	, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_Listbox_BorderSize			, ImGuiStyleVar_FrameBorderSize		, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Listbox_Rounding			, ImGuiStyleVar_FrameRounding		, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Listbox_PaddingX			, ImGuiStyleVar_FramePadding		, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Listbox_PaddingY			, ImGuiStyleVar_FramePadding		, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Listbox_ItemSpacingX		, ImGuiStyleVar_ItemSpacing			, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Listbox_ItemSpacingY		, ImGuiStyleVar_ItemSpacing			, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_Listbox_ScrollbarSize		, ImGuiStyleVar_ScrollbarSize		, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Listbox_ScrollbarRounding	, ImGuiStyleVar_ScrollbarRounding	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Listbox_TextAlignX			, ImGuiStyleVar_SelectableTextAlign	, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_Listbox_TextAlignY			, ImGuiStyleVar_SelectableTextAlign	, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_ADD_CONSTANT(mvThemeCol_Listbox_Text,					mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Listbox_Selected,				mvColor( 60, 150, 250,  79), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Listbox_Hovered,					mvColor( 66, 150, 250, 102), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Listbox_Active,					mvColor( 66, 150, 250, 171), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Listbox_Bg,						mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Listbox_Border,					mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Listbox_Scrollbar,				mvColor(  5,   5,   5, 135), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Listbox_ScrollbarGrab,			mvColor( 79,  79,  79, 255), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Listbox_ScrollbarGrabHovered,	mvColor(105, 105, 105, 255), mvColor(128, 128, 128, 63)),
			MV_ADD_CONSTANT(mvThemeCol_Listbox_ScrollbarGrabActive,		mvColor(130, 130, 130, 255), mvColor(128, 128, 128, 63)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT(mvThemeStyle_Listbox_BorderSize			, 0,  1),
			MV_ADD_CONSTANT(mvThemeStyle_Listbox_Rounding			, 0, 12),
			MV_ADD_CONSTANT(mvThemeStyle_Listbox_PaddingX			, 4, 20),
			MV_ADD_CONSTANT(mvThemeStyle_Listbox_PaddingY			, 3, 20),
			MV_ADD_CONSTANT(mvThemeStyle_Listbox_ItemSpacingX		, 8, 20),
			MV_ADD_CONSTANT(mvThemeStyle_Listbox_ItemSpacingY		, 4, 20),
			MV_ADD_CONSTANT(mvThemeStyle_Listbox_ScrollbarSize		,14, 20),
			MV_ADD_CONSTANT(mvThemeStyle_Listbox_ScrollbarRounding	, 9, 12),
			MV_ADD_CONSTANT(mvThemeStyle_Listbox_TextAlignX			, 0,  1),
			MV_ADD_CONSTANT(mvThemeStyle_Listbox_TextAlignY			, 0,  1),
		MV_END_STYLE_CONSTANTS

	public:

		mvListbox(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		std::vector<std::string> m_names;
		int                      m_itemsHeight = 3; // number of items to show (default -1)
		std::vector<const char*> m_charNames;

	};

}
