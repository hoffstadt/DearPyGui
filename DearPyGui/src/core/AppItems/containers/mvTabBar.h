#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvTabBar, MV_ITEM_DESC_CONTAINER, StorageValueTypes::String, 1);
	class mvTabBar : public mvAppItem
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
		void setValue(mvUUID value);
		void draw(ImDrawList* drawlist, float x, float y) override;
		void setDataSource(mvUUID dataSource) override;
		mvValueVariant getValue() override { return _value; }
		PyObject* getPyValue() override;
		void setPyValue(PyObject* value) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:
		mvRef<mvUUID>    _value = CreateRef<mvUUID>(0);
		mvUUID           _disabled_value = 0;
		ImGuiTabBarFlags _flags = ImGuiTabBarFlags_None;
		mvUUID           _lastValue = 0;
		mvUUID           _uiValue = 0; // value suggested from UI

	};

}