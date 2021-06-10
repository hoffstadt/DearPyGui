#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvTabBar, MV_ITEM_DESC_CONTAINER, StorageValueTypes::String, 1);
	class mvTabBar : public mvUUIDPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTabBar, add_tab_bar)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvTabBar(mvUUID uuid);

		bool canChildBeAdded(mvAppItemType type) override;

		mvUUID getSpecificValue();
		void         setValue(mvUUID value);
		void         draw(ImDrawList* drawlist, float x, float y)               override;

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		ImGuiTabBarFlags m_flags = ImGuiTabBarFlags_None;
		mvUUID      m_lastValue = 0;
		mvUUID      m_uiValue = 0; // value suggested from UI

	};

}