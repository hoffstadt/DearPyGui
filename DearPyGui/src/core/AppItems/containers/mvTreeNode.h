#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvTreeNode, MV_ITEM_DESC_CONTAINER, StorageValueTypes::Bool, 1);
	class mvTreeNode : public mvAppItem
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTreeNode, add_tree_node)
		MV_NO_COMMANDS
		MV_DEFAULT_PARENTS
		MV_DEFAULT_CHILDREN
		MV_NO_CONSTANTS

	public:

		mvTreeNode(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;
		void setDataSource(mvUUID dataSource) override;
		mvValueVariant getValue() override { return _value; }
		PyObject* getPyValue() override;
		void setPyValue(PyObject* value) override;
		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:
		mvRef<bool>        _value = CreateRef<bool>(false);
		bool               _disabled_value = false;
		ImGuiTreeNodeFlags _flags = ImGuiTreeNodeFlags_None;
		bool               _selectable = false;

	};

}