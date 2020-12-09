#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	class mvTreeNode : public mvBoolPtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::TreeNode, "add_tree_node")

		mvTreeNode(const std::string& name);

		void draw              ()               override;
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		ImGuiTreeNodeFlags m_flags = ImGuiTreeNodeFlags_None;

	};

}