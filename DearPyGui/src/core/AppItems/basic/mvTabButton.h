#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	struct mvTabButtonConfig : public mvAppItemConfig
	{
		bool no_reorder = false;
		bool leading = false;
		bool trailing = false;
		bool no_tooltip = false;
	};

	PyObject* add_tab_button(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvTabButton : public mvAppItem
	{


		MV_APPITEM_TYPE(mvAppItemType::TabButton, mvTabButton, "add_tab_button")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::TabButton, mvThemeCol_TabButton_Text		,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TabButton, mvThemeCol_TabButton_Bg			, 33L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TabButton, mvThemeCol_TabButton_BgHovered	, 34L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TabButton, mvThemeCol_TabButton_Border		,  5L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TabButton, mvThemeCol_TabButton_PopupBg		,  4L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TabButton, mvThemeStyle_TabButton_Rounding	, 11L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TabButton, mvThemeStyle_TabButton_BorderSize, 12L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TabButton, mvThemeStyle_TabButton_PaddingX	, 10L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TabButton, mvThemeStyle_TabButton_PaddingY	, 10L, 1L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TabButton_Text, mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TabButton_Bg, mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TabButton_BgHovered, mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TabButton_Border, mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TabButton_PopupBg, mvColor(255, 255, 255, 255)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_TabButton_Rounding	, 0, 12),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_TabButton_BorderSize	, 0, 1),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_TabButton_PaddingX	, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_TabButton_PaddingY	, 0, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvTabButton(const std::string& name);

		void draw()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

		// cpp interface
		void updateConfig(mvAppItemConfig* config) override;
		mvAppItemConfig* getConfig() override;

	private:

		ImGuiTabItemFlags m_flags = ImGuiTabItemFlags_None;
		mvTabButtonConfig m_config;
	};
}