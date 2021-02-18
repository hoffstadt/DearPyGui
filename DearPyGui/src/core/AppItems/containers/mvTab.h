#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	struct mvTabConfig : public mvAppItemConfig
	{
		bool closable = false;
		bool no_reorder = false;
		bool leading = false;
		bool trailing = false;
		bool no_tooltip = false;
	};

	PyObject* add_tab(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvTab : public mvBoolPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::TabItem, mvTab, "add_tab")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::TabItem, mvThemeCol_Tab_Text			,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TabItem, mvThemeCol_Tab_Border			,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TabItem, mvThemeCol_Tab_Bg				, 33L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TabItem, mvThemeCol_Tab_BgHovered		, 34L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TabItem, mvThemeCol_Tab_BgActive		, 35L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TabItem, mvThemeCol_Tab_PopupBg			,  4L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TabItem, mvThemeStyle_Tab_Rounding		, 21L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TabItem, mvThemeStyle_Tab_PaddingX		, 10L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TabItem, mvThemeStyle_Tab_PaddingY		, 10L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TabItem, mvThemeStyle_Tab_InnerSpacingX	, 14L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TabItem, mvThemeStyle_Tab_InnerSpacingY	, 14L, 1L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Tab_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Tab_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Tab_Bg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Tab_BgHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Tab_BgActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Tab_PopupBg),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Tab_Rounding		, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Tab_PaddingX		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Tab_PaddingY		, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Tab_InnerSpacingX	, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_Tab_InnerSpacingY	, 0, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvTab(const std::string& name);

		void draw              ()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		bool              m_closable = false;
		ImGuiTabItemFlags m_flags = ImGuiTabItemFlags_None;

	};

}