#include "mvTreeNode.h"
#include "core/mvInput.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {
	void mvTreeNode::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ s_command, mvPythonParser({
			{mvPythonDataType::Optional},
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
		: mvBoolPtrBase(name)
	{
	}

	void mvTreeNode::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		ImGui::BeginGroup();
		if (ImGui::TreeNodeEx(m_label.c_str(), m_flags))
		{
			//we do this so that the children dont get the theme
			scope.cleanup();

			for (auto& item : m_children[1])
			{
				// skip item if it's not shown
				if (!item->m_show)
					continue;

				// set item width
				if (item->m_width != 0)
					ImGui::SetNextItemWidth((float)item->m_width);

				item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

				item->getState().update();
			}
			ImGui::TreePop();
		}

		ImGui::EndGroup();
	}

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

}