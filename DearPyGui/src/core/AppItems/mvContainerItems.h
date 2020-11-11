#pragma once

#include "core/AppItems/mvTypeBases.h"
#include "mvEventHandler.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvChild
//     * mvGroup
//     * mvCollapsingHeader
//     * mvTreeNode
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvChild
	//-----------------------------------------------------------------------------
	class mvChild : public mvBoolPtrBase, public mvEventHandler
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Child, "add_child")

		mvChild(const std::string& name);

		void draw              ()               override;
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;
		void addFlag           (ImGuiWindowFlags flag);
		void removeFlag        (ImGuiWindowFlags flag);

	private:

		bool             m_border = true;
		bool             m_autosize_x = false;
		bool             m_autosize_y = false;
		ImGuiWindowFlags m_windowflags = ImGuiWindowFlags_NoSavedSettings;

	};

	//-----------------------------------------------------------------------------
	// mvGroup
	//-----------------------------------------------------------------------------
	class mvGroup : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Group, "add_group")

		mvGroup(const std::string& name);

		void draw              ()               override;
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		bool  m_horizontal = false;
		float m_hspacing = -1.0f;

	};

	//-----------------------------------------------------------------------------
	// mvCollapsingHeader
	//-----------------------------------------------------------------------------
	class mvCollapsingHeader : public mvBoolPtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::CollapsingHeader, "add_collapsing_header")

		mvCollapsingHeader(const std::string& name);

		void draw              ()               override;
		void setExtraConfigDict(PyObject* dict) override;
		void getExtraConfigDict(PyObject* dict) override;

	private:

		ImGuiTreeNodeFlags m_flags = ImGuiTreeNodeFlags_None;
		bool               m_closable = false;

	};

	//-----------------------------------------------------------------------------
	// mvTreeNode
	//-----------------------------------------------------------------------------
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