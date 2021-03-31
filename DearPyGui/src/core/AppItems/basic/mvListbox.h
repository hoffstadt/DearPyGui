#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	struct mvListboxConfig : public mvAppItemConfig
	{
		std::vector<std::string> items;
		int default_value = 0;
		int num_items = 3;

	};

	MV_REGISTER_WIDGET(mvListbox);
	class mvListbox : public mvIntPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::mvListbox, add_listbox)

		MV_CREATE_THEME_CONSTANT(mvThemeCol_Listbox_Text                ,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Listbox_Selected            , 24L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Listbox_Hovered             , 25L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Listbox_Active              , 26L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Listbox_Bg                  ,  7L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Listbox_Border              ,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Listbox_Scrollbar           , 14L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Listbox_ScrollbarGrab       , 15L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Listbox_ScrollbarGrabHovered, 16L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Listbox_ScrollbarGrabActive , 17L, 0L);

		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Listbox_BorderSize		, 12L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Listbox_Rounding			, 11L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Listbox_PaddingX			, 10L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Listbox_PaddingY			, 10L, 1L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Listbox_ItemSpacingX		, 13L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Listbox_ItemSpacingY		, 13L, 1L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Listbox_ScrollbarSize		, 17L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Listbox_ScrollbarRounding	, 18L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Listbox_TextAlignX		, 23L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Listbox_TextAlignY		, 23L, 1L);

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeCol_Listbox_Text,					mvColor(255, 255, 255, 255), mvColor(128, 128, 128, 63)),
			MV_CREATE_CONSTANT_TUPLE(mvThemeCol_Listbox_Selected,				mvColor( 60, 150, 250,  79), mvColor(128, 128, 128, 63)),
			MV_CREATE_CONSTANT_TUPLE(mvThemeCol_Listbox_Hovered,				mvColor( 66, 150, 250, 102), mvColor(128, 128, 128, 63)),
			MV_CREATE_CONSTANT_TUPLE(mvThemeCol_Listbox_Active,					mvColor( 66, 150, 250, 171), mvColor(128, 128, 128, 63)),
			MV_CREATE_CONSTANT_TUPLE(mvThemeCol_Listbox_Bg,						mvColor( 41,  74, 122, 138), mvColor(128, 128, 128, 63)),
			MV_CREATE_CONSTANT_TUPLE(mvThemeCol_Listbox_Border,					mvColor(110, 110, 128, 128), mvColor(128, 128, 128, 63)),
			MV_CREATE_CONSTANT_TUPLE(mvThemeCol_Listbox_Scrollbar,				mvColor(  5,   5,   5, 135), mvColor(128, 128, 128, 63)),
			MV_CREATE_CONSTANT_TUPLE(mvThemeCol_Listbox_ScrollbarGrab,			mvColor( 79,  79,  79, 255), mvColor(128, 128, 128, 63)),
			MV_CREATE_CONSTANT_TUPLE(mvThemeCol_Listbox_ScrollbarGrabHovered,	mvColor(105, 105, 105, 255), mvColor(128, 128, 128, 63)),
			MV_CREATE_CONSTANT_TUPLE(mvThemeCol_Listbox_ScrollbarGrabActive,	mvColor(130, 130, 130, 255), mvColor(128, 128, 128, 63)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Listbox_BorderSize		, 0,  1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Listbox_Rounding			, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Listbox_PaddingX			, 4, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Listbox_PaddingY			, 3, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Listbox_ItemSpacingX		, 8, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Listbox_ItemSpacingY		, 4, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Listbox_ScrollbarSize		,14, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Listbox_ScrollbarRounding	, 9, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Listbox_TextAlignX		, 0,  1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Listbox_TextAlignY		, 0,  1),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvListbox(const std::string& name, int default_value, const std::string& dataSource);

		void draw()               override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

		// cpp interface
		void updateConfig(mvAppItemConfig* config) override;
		mvAppItemConfig* getConfig() override;

	private:

		std::vector<std::string> m_names;
		int                      m_itemsHeight = 3; // number of items to show (default -1)
		std::vector<const char*> m_charNames;
		mvListboxConfig          m_config;

	};

}
