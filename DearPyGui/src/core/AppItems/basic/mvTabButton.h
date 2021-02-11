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

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TabButton_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TabButton_Bg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TabButton_BgHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TabButton_Border),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TabButton_PopupBg),
		MV_END_COLOR_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvTabButton(const std::string& name);

		void draw()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		ImGuiTabItemFlags m_flags = ImGuiTabItemFlags_None;
	};
}