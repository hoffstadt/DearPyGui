#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	struct mvTreeNodeConfig : public mvAppItemConfig
	{
		bool default_open = false;
		bool open_on_double_click = false;
		bool open_on_arrow = false;
		bool leaf = false;
		bool bullet = false;
	};

	MV_REGISTER_WIDGET(mvTreeNode);
	class mvTreeNode : public mvBoolPtrBase
	{
		MV_APPITEM_TYPE(mvAppItemType::mvTreeNode, add_tree_node)

		MV_CREATE_CONSTANT(mvThemeCol_TreeNode_Text				,  0L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_TreeNode_BgHovered			, 25L, 0L);
		MV_CREATE_CONSTANT(mvThemeCol_TreeNode_BgActive			, 26L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_TreeNode_FramePaddingX	, 10L, 0L);
		MV_CREATE_CONSTANT(mvThemeStyle_TreeNode_FramePaddingY	, 10L, 1L);
		MV_CREATE_CONSTANT(mvThemeStyle_TreeNode_IndentSpacing	, 15L, 0L);

		MV_START_EXTRA_COMMANDS
		MV_END_EXTRA_COMMANDS

		MV_START_GENERAL_CONSTANTS
		MV_END_GENERAL_CONSTANTS

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TreeNode_Text,		mvColor(255, 255, 255, 255)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TreeNode_BgHovered,	mvColor( 66, 150, 250, 204)),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TreeNode_BgActive,	mvColor( 66, 150, 250, 255)),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_ADD_CONSTANT(mvThemeStyle_TreeNode_FramePaddingX, 4, 20),
			MV_ADD_CONSTANT(mvThemeStyle_TreeNode_FramePaddingY, 3, 20),
			MV_ADD_CONSTANT(mvThemeStyle_TreeNode_IndentSpacing,21, 30),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvTreeNode(const std::string& name);

		void draw              ()               override;

		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		ImGuiTreeNodeFlags m_flags = ImGuiTreeNodeFlags_None;

	};

}