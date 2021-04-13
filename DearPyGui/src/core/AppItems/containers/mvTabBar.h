#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvTabBar, MV_ITEM_DESC_CONTAINER, StorageValueTypes::String, 1);
	class mvTabBar : public mvStringPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTabBar, add_tab_bar)

		MV_CREATE_CONSTANT(mvThemeCol_TabBar, 35L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_TabBar_ItemSpacingX, ImGuiStyleVar_ItemSpacing, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_TabBar_ItemSpacingY, ImGuiStyleVar_ItemSpacing, 1L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TabBar, mvColor(110, 110, 128, 128)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT_F(mvThemeStyle_TabBar_ItemSpacingX, 8, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_TabBar_ItemSpacingY, 4, 20),
		MV_END_STYLE_CONSTANTS

	public:

		mvTabBar(const std::string& name);

		bool canChildBeAdded(mvAppItemType type) override;

		std::string& getSpecificValue();
		void         setValue(const std::string& value);
		void         draw(ImDrawList* drawlist, float x, float y)               override;

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		ImGuiTabBarFlags m_flags = ImGuiTabBarFlags_None;
		std::string      m_lastValue;
		std::string      m_uiValue; // value suggested from UI

	};

}