#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvTabBar, MV_ITEM_DESC_CONTAINER, StorageValueTypes::String, 1);
	class mvTabBar : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTabBar, add_tab_bar)
		MV_NO_COMMANDS
		MV_DEFAULT_PARENTS
		MV_NO_CONSTANTS

		MV_SET_STATES(MV_STATE_NONE);

		MV_START_CHILDREN
			MV_ADD_CHILD(mvAppItemType::mvTab),
			MV_ADD_CHILD(mvAppItemType::mvTabButton),
			MV_ADD_CHILD(mvAppItemType::mvActivatedHandler),
			MV_ADD_CHILD(mvAppItemType::mvActiveHandler),
			MV_ADD_CHILD(mvAppItemType::mvClickedHandler),
			MV_ADD_CHILD(mvAppItemType::mvDeactivatedAfterEditHandler),
			MV_ADD_CHILD(mvAppItemType::mvDeactivatedHandler),
			MV_ADD_CHILD(mvAppItemType::mvEditedHandler),
			MV_ADD_CHILD(mvAppItemType::mvFocusHandler),
			MV_ADD_CHILD(mvAppItemType::mvHoverHandler),
			MV_ADD_CHILD(mvAppItemType::mvResizeHandler),
			MV_ADD_CHILD(mvAppItemType::mvToggledOpenHandler),
			MV_ADD_CHILD(mvAppItemType::mvVisibleHandler)
		MV_END_CHILDREN

	public:

		explicit mvTabBar(mvUUID uuid);

		mvUUID getSpecificValue();
		void setValue(mvUUID value);
		void draw(ImDrawList* drawlist, float x, float y) override;
		void setDataSource(mvUUID dataSource) override;
		void* getValue() override { return &_value; }
		PyObject* getPyValue() override;
		void setPyValue(PyObject* value) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;
		void applySpecificTemplate(mvAppItem* item) override;

	private:
		mvRef<mvUUID>    _value = CreateRef<mvUUID>(0);
		mvUUID           _disabled_value = 0;
		ImGuiTabBarFlags _flags = ImGuiTabBarFlags_None;
		mvUUID           _lastValue = 0;
		mvUUID           _uiValue = 0; // value suggested from UI

	};

}