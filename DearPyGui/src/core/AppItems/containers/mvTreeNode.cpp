#include "mvTreeNode.h"
#include "core/mvInput.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvThemeManager.h"

namespace Marvel {
	void mvTreeNode::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_tree_node", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "default_open", "", "False"},
			{mvPythonDataType::Bool, "open_on_double_click", "Need double-click to open node", "False"},
			{mvPythonDataType::Bool, "open_on_arrow", "Only open when clicking on the arrow part.", "False"},
			{mvPythonDataType::Bool, "leaf", "No collapsing, no arrow (use as a convenience for leaf nodes).", "False"},
			{mvPythonDataType::Bool, "bullet", "Display a bullet instead of arrow", "False"},
		}, "Adds a tree node to add items to. Must be closed with the end command.",
		"None", "Containers") });
	}
	mvTreeNode::mvTreeNode(const std::string& name)
		: mvBoolPtrBase(name, false)
	{
		m_description.container = true;
	}

	void mvTreeNode::draw()
	{
		auto styleManager = m_styleManager.getScopedStyleManager();
		ScopedID id;
		mvImGuiThemeScope scope(this);

		ImGui::BeginGroup();
		if (ImGui::TreeNodeEx(m_label.c_str(), m_flags))
		{
			//we do this so that the children dont get the theme
			scope.cleanup();

			for (auto& item : m_children)
			{
				// skip item if it's not shown
				if (!item->m_core_config.show)
					continue;

				// set item width
				if (item->m_core_config.width != 0)
					ImGui::SetNextItemWidth((float)item->m_core_config.width);

				item->draw();

				item->getState().update();
			}
			ImGui::TreePop();
		}

		ImGui::EndGroup();
	}

#ifndef MV_CPP

	void mvTreeNode::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// flags
		flagop("default_open", ImGuiTreeNodeFlags_DefaultOpen, m_flags);
		flagop("open_on_double_click", ImGuiTreeNodeFlags_OpenOnDoubleClick, m_flags);
		flagop("open_on_arrow", ImGuiTreeNodeFlags_OpenOnArrow, m_flags);
		flagop("leaf", ImGuiTreeNodeFlags_Leaf, m_flags);
		flagop("bullet", ImGuiTreeNodeFlags_Bullet, m_flags);
	}

	void mvTreeNode::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		// flags
		checkbitset("default_open", ImGuiTreeNodeFlags_DefaultOpen, m_flags);
		checkbitset("open_on_double_click", ImGuiTreeNodeFlags_OpenOnDoubleClick, m_flags);
		checkbitset("open_on_arrow", ImGuiTreeNodeFlags_OpenOnArrow, m_flags);
		checkbitset("leaf", ImGuiTreeNodeFlags_Leaf, m_flags);
		checkbitset("bullet", ImGuiTreeNodeFlags_Bullet, m_flags);

	}

	PyObject* add_tree_node(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* label = "";
		int show = false;
		const char* parent = "";
		const char* before = "";
		int default_open = false;
		int open_on_double_click = false;
		int open_on_arrow = false;
		int leaf = false;
		int bullet = false;

		if (!(mvApp::GetApp()->getParsers())["add_tree_node"].parse(args, kwargs, __FUNCTION__, &name,
			&label, &show, &parent, &before, &default_open, &open_on_double_click, &open_on_arrow, &leaf, &bullet))
			return ToPyBool(false);

		auto item = CreateRef<mvTreeNode>(name);
		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		if (mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before))
		{
			mvApp::GetApp()->getItemRegistry().pushParent(item);
			if (!show)
				item->hide();

		}

		return GetPyNone();
	}

#endif
}