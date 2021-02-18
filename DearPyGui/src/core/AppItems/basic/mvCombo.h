#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	struct mvComboConfig : public mvAppItemConfig
	{	
		std::vector<std::string> items;
		std::string default_value = "";
		bool popup_align_left = false;
		bool height_small = false;
		bool height_regular = false;
		bool height_large = false;
		bool height_largest = false;
		bool no_arrow_button = false;
		bool no_preview = false;
	};

	PyObject* add_combo(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvCombo : public mvStringPtrBase
	{
		MV_APPITEM_TYPE(mvAppItemType::Combo, mvCombo, "add_combo")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeCol_Combo_Text                ,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeCol_Combo_Selected            , 24L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeCol_Combo_Hovered             , 25L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeCol_Combo_Active              , 26L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeCol_Combo_Bg                  ,  7L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeCol_Combo_BgHovered           ,  8L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeCol_Combo_DropBg              ,  4L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeCol_Combo_DropButtonBg        , 21L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeCol_Combo_DropButtonHovered   , 22L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeCol_Combo_Scrollbar           , 14L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeCol_Combo_ScrollbarGrab       , 15L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeCol_Combo_ScrollbarGrabHovered, 16L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeCol_Combo_ScrollbarGrabActive , 17L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeCol_Combo_Border              ,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeCol_Combo_BorderShadow        ,  6L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeStyle_Combo_BorderSize		, 12L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeStyle_Combo_Rounding			, 11L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeStyle_Combo_PaddingX			, 10L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeStyle_Combo_PaddingY			, 10L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeStyle_Combo_DropPaddingX		,  1L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeStyle_Combo_DropPaddingY		,  1L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeStyle_Combo_DropBorderSize	,  9L, 0L)
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeStyle_Combo_DropRounding		,  8L, 0L)
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeStyle_Combo_ItemSpacingX		, 13L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeStyle_Combo_ItemSpacingY		, 13L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeStyle_Combo_ScrollbarSize		, 17L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeStyle_Combo_ScrollbarRounding	, 18L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeStyle_Combo_TextAlignX		, 23L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Combo, mvThemeStyle_Combo_TextAlignY		, 23L, 1L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_Selected),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_Hovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_Active),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_Bg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_BgHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_DropBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_DropButtonBg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_DropButtonHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_Scrollbar),           
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_ScrollbarGrab),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_ScrollbarGrabHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_ScrollbarGrabActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_BorderShadow),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Combo_BorderSize			, 0,  1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Combo_Rounding			, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Combo_PaddingX			, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Combo_PaddingY			, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Combo_DropPaddingX		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Combo_DropPaddingY		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Combo_DropBorderSize		, 0,  1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Combo_DropRounding		, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Combo_ItemSpacingX		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Combo_ItemSpacingY		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Combo_ScrollbarSize		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Combo_ScrollbarRounding	, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Combo_TextAlignX			, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Combo_TextAlignY			, 0, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvCombo(const std::string& name, const std::string& default_value, const std::string& dataSource);

		void draw()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:
		ImGuiComboFlags m_flags = ImGuiComboFlags_None;
		std::vector<std::string> m_items;
	};

}
