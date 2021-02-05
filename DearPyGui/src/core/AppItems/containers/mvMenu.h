#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_menu(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvMenu : public mvBoolPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::Menu, mvMenu, "add_menu")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::Menu, mvThemeCol_Menu_Text		,  0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Menu, mvThemeCol_Menu_Bg		,  4L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Menu, mvThemeCol_Menu_BgHovered	, 25L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Menu, mvThemeCol_Menu_BgActive	, 24L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::Menu, mvThemeCol_Menu_Border	,  5L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Menu_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Menu_Bg),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Menu_BgHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Menu_BgActive),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Menu_Border),
		MV_END_COLOR_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvMenu(const std::string& name);

		void draw() override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	};

}