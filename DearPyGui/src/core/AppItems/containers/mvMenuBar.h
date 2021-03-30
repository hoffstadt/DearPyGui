#pragma once

#include "mvTypeBases.h"
#include "mvAppItem.h"

namespace Marvel {

	struct mvMenuBarConfig : public mvAppItemConfig {};

	PyObject* add_menu_bar(PyObject* self, PyObject* args, PyObject* kwargs);

	MV_REGISTER_WIDGET(mvMenuBar);
	class mvMenuBar : public mvBoolPtrBase
	{
		MV_APPITEM_TYPE(mvAppItemType::mvMenuBar, "add_menu_bar")
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_MenuBar_ItemSpacingX, 13L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_MenuBar_ItemSpacingY, 13L, 1L);

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_MenuBar_ItemSpacingX, 8, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_MenuBar_ItemSpacingY, 4, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		explicit mvMenuBar(const std::string& name);

		void draw() override;

	};

}