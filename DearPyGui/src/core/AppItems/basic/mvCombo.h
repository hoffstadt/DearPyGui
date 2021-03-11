#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvColorPicker Config Struct
	//-----------------------------------------------------------------------------
	struct mvComboConfig : public mvAppItemConfig
	{	
		std::vector<std::string> items;
		std::string              default_value    = "";
		bool                     popup_align_left = false;
		bool                     height_small     = false;
		bool                     height_regular   = false;
		bool                     height_large     = false;
		bool                     height_largest   = false;
		bool                     no_arrow_button  = false;
		bool                     no_preview       = false;
	};

#ifdef MV_CPP
	void add_combo(const char* name, const mvComboConfig& config = {});
	void add_combo(const char* name, const std::vector<std::string>& items, mvCallable callable = nullptr);
#else
	PyObject* add_combo(PyObject* self, PyObject* args, PyObject* kwargs);
#endif

	MV_REGISTER_WIDGET(mvCombo);
	class mvCombo : public mvStringPtrBase
	{
		MV_APPITEM_TYPE(mvAppItemType::mvCombo, "add_combo")

		MV_CREATE_THEME_CONSTANT(mvThemeCol_Combo_Text,                  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Combo_Selected,             24L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Combo_Hovered,              25L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Combo_Active,               26L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Combo_Bg,                    7L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Combo_BgHovered,             8L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Combo_DropBg,                4L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Combo_DropButtonBg,         21L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Combo_DropButtonHovered,    22L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Combo_Scrollbar,            14L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Combo_ScrollbarGrab,        15L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Combo_ScrollbarGrabHovered, 16L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Combo_ScrollbarGrabActive,  17L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Combo_Border,                5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeCol_Combo_BorderShadow,          6L, 0L);

		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Combo_BorderSize,         12L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Combo_Rounding,           11L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Combo_PaddingX,           10L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Combo_PaddingY,           10L, 1L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Combo_DropPaddingX,        1L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Combo_DropPaddingY,        1L, 1L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Combo_DropBorderSize,      9L, 0L)
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Combo_DropRounding,        8L, 0L)
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Combo_ItemSpacingX,       13L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Combo_ItemSpacingY,       13L, 1L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Combo_ScrollbarSize,      17L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Combo_ScrollbarRounding,  18L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Combo_TextAlignX,         23L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_Combo_TextAlignY,         23L, 1L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_Text,                 mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_Selected,             mvColor( 66, 150, 250, 171)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_Hovered,              mvColor( 41,  74,  74, 138)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_Active,               mvColor( 66, 150, 250, 171)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_Bg,                   mvColor( 41,  74, 122, 138)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_BgHovered,            mvColor( 41,  74,  74, 138)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_DropBg,               mvColor( 20,  20,  20, 240)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_DropButtonBg,         mvColor( 66, 150, 250, 102)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_DropButtonHovered,    mvColor( 66, 150, 250, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_Scrollbar,            mvColor(  5,   5,   5, 135)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_ScrollbarGrab,        mvColor( 79,  79,  79, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_ScrollbarGrabHovered, mvColor(105, 105, 105, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_ScrollbarGrabActive,  mvColor(130, 130, 130, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_Border,               mvColor(110, 110, 128, 128)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Combo_BorderShadow,         mvColor(  0,   0,   0,   0)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Combo_BorderSize			, 0,    1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Combo_Rounding			, 0,   12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Combo_PaddingX			, 4,   20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Combo_PaddingY			, 3,   20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Combo_DropPaddingX		, 8,   20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Combo_DropPaddingY		, 8,   20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Combo_DropBorderSize		, 1,    1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Combo_DropRounding		, 0,   12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Combo_ItemSpacingX		, 8,   20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Combo_ItemSpacingY		, 4,   20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Combo_ScrollbarSize		,14,   20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Combo_ScrollbarRounding	, 9,   12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Combo_TextAlignX			, 0.5, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Combo_TextAlignY			, 0.5, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvCombo(const std::string& name, const std::string& default_value, const std::string& dataSource);
		mvCombo(const std::string& name, const mvComboConfig& config);

		void draw() override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

		// cpp interface
		void updateConfig(mvAppItemConfig* config) override;
		mvAppItemConfig* getConfig() override;

	private:
		ImGuiComboFlags m_flags = ImGuiComboFlags_None;
		mvComboConfig   m_config;
	};

}
