#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	PyObject* add_tree_node(PyObject* self, PyObject* args, PyObject* kwargs);

	class mvTreeNode : public mvBoolPtrBase
	{

	public:

		static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

	public:

		MV_APPITEM_TYPE_OLD_SYSTEM(mvAppItemType::TreeNode, "add_tree_node")

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