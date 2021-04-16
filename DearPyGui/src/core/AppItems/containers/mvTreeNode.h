#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	MV_REGISTER_WIDGET(mvTreeNode, MV_ITEM_DESC_CONTAINER, StorageValueTypes::Bool, 1);
	class mvTreeNode : public mvBoolPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

		MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvTreeNode, add_tree_node)

		MV_CREATE_CONSTANT(mvThemeCol_TreeNode_Text, ImGuiCol_Text, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_TreeNode_BgHovered, ImGuiCol_HeaderHovered, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_TreeNode_BgActive, ImGuiCol_HeaderActive, 0L);

		MV_CREATE_CONSTANT(mvThemeStyle_TreeNode_FramePaddingX, ImGuiStyleVar_FramePadding, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_TreeNode_FramePaddingY, ImGuiStyleVar_FramePadding, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_TreeNode_IndentSpacing, ImGuiStyleVar_IndentSpacing, 0L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TreeNode_Text, mvImGuiCol_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TreeNode_BgHovered, mvImGuiCol_HeaderHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TreeNode_BgActive, mvImGuiCol_ButtonHovered),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT_F(mvThemeStyle_TreeNode_FramePaddingX, 4, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_TreeNode_FramePaddingY, 3, 20),
			MV_ADD_CONSTANT_F(mvThemeStyle_TreeNode_IndentSpacing,21, 30),
		MV_END_STYLE_CONSTANTS

	public:

		mvTreeNode(const std::string& name);

		void draw(ImDrawList* drawlist, float x, float y) override;

		void handleSpecificKeywordArgs(PyObject* dict) override;
		void getSpecificConfiguration(PyObject* dict) override;

	private:

		ImGuiTreeNodeFlags m_flags = ImGuiTreeNodeFlags_None;

	};

}