#pragma once

#include "mvTypeBases.h"
#include "mvAppItems.h"

namespace Marvel {

	struct mvMenuBarConfig : public mvAppItemConfig {};

	PyObject* add_menu_bar(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvMenuBar : public mvBoolPtrBase
	{
		MV_APPITEM_TYPE(mvAppItemType::MenuBar, mvMenuBar, "add_menu_bar")
		MV_CREATE_THEME_CONSTANT(mvAppItemType::MenuBar, mvThemeStyle_MenuBar_ItemSpacingX, 13L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::MenuBar, mvThemeStyle_MenuBar_ItemSpacingY, 13L, 1L);

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_MenuBar_ItemSpacingX, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_MenuBar_ItemSpacingY, 0, 20),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		explicit mvMenuBar(const std::string& name);

		void draw() override;

	};

}