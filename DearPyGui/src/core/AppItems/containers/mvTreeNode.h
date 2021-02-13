#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_tree_node(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvTreeNode : public mvBoolPtrBase
	{
		MV_APPITEM_TYPE(mvAppItemType::TreeNode, mvTreeNode, "add_tree_node")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::TreeNode, mvThemeCol_TreeNode_Text				,  0L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TreeNode, mvThemeCol_TreeNode_BgHovered			, 25L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TreeNode, mvThemeCol_TreeNode_BgActive			, 26L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TreeNode, mvThemeStyle_TreeNode_FramePaddingX	, 10L, 0L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TreeNode, mvThemeStyle_TreeNode_FramePaddingY	, 10L, 1L);
		MV_CREATE_THEME_CONSTANT(mvAppItemType::TreeNode, mvThemeStyle_TreeNode_IndentSpacing	, 15L, 0L);


		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TreeNode_Text),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TreeNode_BgHovered),
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_TreeNode_BgActive),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_TreeNode_FramePaddingX, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_TreeNode_FramePaddingY, 0, 20),
			MV_CREATE_CONSTANT_TUPLE(mvThemeStyle_TreeNode_IndentSpacing, 0, 30),
		MV_END_STYLE_CONSTANTS

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		mvTreeNode(const std::string& name);

		void draw              ()               override;

#ifndef MV_CPP
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
#endif // !MV_CPP

	private:

		ImGuiTreeNodeFlags m_flags = ImGuiTreeNodeFlags_None;

	};

}