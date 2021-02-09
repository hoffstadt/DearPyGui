#pragma once

#include "mvTypeBases.h"
#include "mvApp.h"

namespace Marvel {

	PyObject* add_menu_item(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvMenuItem : public mvBoolPtrBase
	{
		MV_APPITEM_TYPE(mvAppItemType::MenuItem, mvMenuItem, "add_menu_item")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::MenuItem, mvThemeCol_MenuItem_Text		,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::MenuItem, mvThemeCol_MenuItem_BgHovered	, 25L, 0L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_MenuItem_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_MenuItem_BgHovered),
		MV_END_COLOR_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		explicit mvMenuItem(const std::string& name);

		void draw() override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		std::string m_shortcut;
		bool        m_check = false;

	};

}