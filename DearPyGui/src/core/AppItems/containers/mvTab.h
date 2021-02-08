#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_tab(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvTab : public mvBoolPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::TabItem, mvTab, "add_tab")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::TabItem, mvThemeCol_Tab_Text		,  0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TabItem, mvThemeCol_Tab_Bg			, 33L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TabItem, mvThemeCol_Tab_BgHovered	, 34L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TabItem, mvThemeCol_Tab_BgActive	, 35L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TabItem, mvThemeCol_Tab_PopupBg		,  4L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Tab_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Tab_Bg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Tab_BgHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Tab_BgActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Tab_PopupBg),
		MV_END_COLOR_CONSTANTS

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