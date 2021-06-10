#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvTreeNode, MV_ITEM_DESC_CONTAINER, StorageValueTypes::Bool, 1);
	class mvTreeNode : public mvBoolPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTreeNode, add_tree_node)

		MV_START_COMMANDS
		MV_END_COMMANDS

		MV_START_CONSTANTS
		MV_END_CONSTANTS

	public:

		mvTreeNode(mvUUID uuid);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		ImGuiTreeNodeFlags m_flags = ImGuiTreeNodeFlags_None;
		bool               m_selectable = false;

	};

}