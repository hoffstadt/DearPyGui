#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	struct mvTabBarConfig : public mvAppItemConfig
	{
		bool reorderable = false;
	};

	PyObject* add_tab_bar(PyObject* self, PyObject* args, PyObject* kwargs);

	MV_REGISTER_WIDGET(mvTabBar);
	class mvTabBar : public mvStringPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::mvTabBar, "add_tab_bar")

		MV_CREATE_THEME_CONSTANT(mvThemeCol_TabBar, 35L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_TabBar_ItemSpacingX, 13L, 0L);
		MV_CREATE_THEME_CONSTANT(mvThemeStyle_TabBar_ItemSpacingY, 13L, 1L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TabBar, mvColor(110, 110, 128, 128)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_TabBar_ItemSpacingX, 8, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_TabBar_ItemSpacingY, 4, 20),
		MV_END_STYLE_CONSTANTS
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvTabBar(const std::string& name);

		std::string& getSpecificValue();
		void         setValue(const std::string& value);
		void         draw()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		ImGuiTabBarFlags m_flags = ImGuiTabBarFlags_None;

	};

}