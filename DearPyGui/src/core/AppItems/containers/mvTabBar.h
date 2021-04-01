#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	struct mvTabBarConfig : public mvAppItemConfig
	{
		bool reorderable = false;
	};

	MV_REGISTER_WIDGET(mvTabBar);
	class mvTabBar : public mvStringPtrBase
	{

		MV_APPITEM_TYPE(mvAppItemType::mvTabBar, add_tab_bar)

		MV_CREATE_CONSTANT(mvThemeCol_TabBar, 35L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_TabBar_ItemSpacingX, 13L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_TabBar_ItemSpacingY, 13L, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TabBar, mvColor(110, 110, 128, 128)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT(mvThemeStyle_TabBar_ItemSpacingX, 8, 20),
			MV_ADD_CONSTANT(mvThemeStyle_TabBar_ItemSpacingY, 4, 20),
		MV_END_STYLE_CONSTANTS
	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvTabBar(const std::string& name);

		std::string& getSpecificValue();
		void         setValue(const std::string& value);
		void         draw()               override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		ImGuiTabBarFlags m_flags = ImGuiTabBarFlags_None;
		std::string      m_lastValue;
		std::string      m_uiValue; // value suggested from UI

	};

}