#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvTab, MV_ITEM_DESC_CONTAINER, StorageValueTypes::Bool, 1);
	class mvTab : public mvAppItem
	{

		enum class TabOrdering {
			mvTabOrder_Reorderable = 0L,
			mvTabOrder_Fixed,
			mvTabOrder_Leading,
			mvTabOrder_Trailing
		};

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTab, add_tab)
		MV_NO_COMMANDS
		MV_DEFAULT_CHILDREN

		MV_CREATE_CONSTANT(mvTabOrder_Reorderable, 0L);
		MV_CREATE_CONSTANT(mvTabOrder_Fixed, 1L);
		MV_CREATE_CONSTANT(mvTabOrder_Leading, 2L);
		MV_CREATE_CONSTANT(mvTabOrder_Trailing, 3L);

		MV_START_PARENTS
			MV_ADD_PARENT(mvAppItemType::mvTabBar),
			MV_ADD_PARENT(mvAppItemType::mvStagingContainer),
			MV_ADD_PARENT(mvAppItemType::mvTemplateRegistry),
		MV_END_PARENTS

		MV_START_CONSTANTS
			MV_ADD_CONSTANT(mvTabOrder_Reorderable),
			MV_ADD_CONSTANT(mvTabOrder_Fixed),
			MV_ADD_CONSTANT(mvTabOrder_Leading),
			MV_ADD_CONSTANT(mvTabOrder_Trailing)
		MV_END_CONSTANTS

	public:

		mvTab(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void setDataSource(mvUUID dataSource) override;
		void* getValue() override { return &_value; }
		PyObject* getPyValue() override;
		void setPyValue(PyObject* value) override;
		void addFlag   (ImGuiTabItemFlags flag);
		void removeFlag(ImGuiTabItemFlags flag);
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		mvRef<bool>       _value = CreateRef<bool>(false);
		bool              _disabled_value = false;
		bool              _closable = false;
		ImGuiTabItemFlags _flags = ImGuiTabItemFlags_None;

	};

}