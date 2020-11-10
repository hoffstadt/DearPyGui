#pragma once

#include "Core/AppItems/mvTypeBases.h"
#include "mvEventHandler.h"
#include "Core/mvInput.h"

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

		mvChild(const std::string& name)
			: mvBoolPtrBase(name, false, name), mvEventHandler()
		{
			m_description.container = true;
		}

		void addFlag   (ImGuiWindowFlags flag) { m_windowflags |= flag; }
		void removeFlag(ImGuiWindowFlags flag) { m_windowflags &= ~flag; }

		void draw() override
		{
			auto styleManager = m_styleManager.getScopedStyleManager();
			ScopedID id;

			ImGui::BeginChild(m_label.c_str(), ImVec2(m_autosize_x ? 0 : (float)m_width, m_autosize_y ? 0 : (float)m_height), m_border, m_windowflags);

			for (mvAppItem* item : m_children)
			{
				// skip item if it's not shown
				if (!item->isShown())
					continue;

				// set item width
				if (item->getWidth() != 0)
					ImGui::SetNextItemWidth((float)item->getWidth());

				item->draw();

				item->getState().update();
			}

			// TODO check if these work for child
			if (!m_tip .empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", m_tip.c_str());

			// allows this item to have a render callback
			registerWindowFocusing();

			m_width = (int)ImGui::GetWindowWidth();
			m_height = (int)ImGui::GetWindowHeight();

			ImGui::EndChild();
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "border")) m_border = ToBool(item);
			if (PyObject* item = PyDict_GetItemString(dict, "autosize_x")) m_autosize_x = ToBool(item);
			if (PyObject* item = PyDict_GetItemString(dict, "autosize_y")) m_autosize_y = ToBool(item);

			// helper for bit flipping
			auto flagop = [dict](const char* keyword, int flag, int& flags)
			{
				if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
			};

			// window flags
			flagop("no_scrollbar", ImGuiWindowFlags_NoScrollbar, m_windowflags);
			flagop("horizontal_scrollbar", ImGuiWindowFlags_HorizontalScrollbar, m_windowflags);
			flagop("menubar", ImGuiWindowFlags_MenuBar, m_windowflags);
		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "border", ToPyBool(m_border));
			PyDict_SetItemString(dict, "autosize_x", ToPyBool(m_autosize_x));
			PyDict_SetItemString(dict, "autosize_y", ToPyBool(m_autosize_y));

			// helper for bit flipping
			auto flagop = [dict](const char* keyword, int flag, int& flags)
			{
				if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
			};

			// window flags
			flagop("no_scrollbar", ImGuiWindowFlags_NoScrollbar, m_windowflags);
			flagop("horizontal_scrollbar", ImGuiWindowFlags_HorizontalScrollbar, m_windowflags);
			flagop("menubar", ImGuiWindowFlags_MenuBar, m_windowflags);
		}

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

		mvGroup(const std::string& name)
			: mvAppItem(name)
		{
			m_description.container = true;
		}

		void draw() override
		{
			auto styleManager = m_styleManager.getScopedStyleManager();

			if (m_width != 0)
				ImGui::PushItemWidth((float)m_width);

			ImGui::BeginGroup();

			for (mvAppItem* item : m_children)
			{
				if (m_width != 0)
					item->setWidth(m_width);

				// skip item if it's not shown
				if (!item->isShown())
					continue;

				// set item width
				if (item->getWidth() != 0)
					ImGui::SetNextItemWidth((float)item->getWidth());

				item->draw();

				if (m_horizontal)
					ImGui::SameLine(0.0, m_hspacing);

				item->getState().update();
			}

			if (m_width != 0)
				ImGui::PopItemWidth();

			ImGui::EndGroup();

			if (!m_tip.empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", m_tip.c_str());

		}

		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "horizontal")) m_horizontal = ToBool(item);
			if (PyObject* item = PyDict_GetItemString(dict, "horizontal_spacing")) m_hspacing = ToFloat(item);
		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "horizontal", ToPyBool(m_horizontal));
			PyDict_SetItemString(dict, "horizontal_spacing", ToPyFloat(m_hspacing));
		}

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

		mvCollapsingHeader(const std::string& name)
			: mvBoolPtrBase(name, true, name)
		{
			m_description.container = true;
		}

		void draw() override
		{
			auto styleManager = m_styleManager.getScopedStyleManager();
			ScopedID id;

			bool* toggle = nullptr;
			if (m_closable)
				toggle = m_value;

			if (ImGui::CollapsingHeader(m_label.c_str(), toggle, m_flags))
			{

				// Regular Tooltip (simple)
				if (!getTip().empty() && ImGui::IsItemHovered())
					ImGui::SetTooltip("%s", getTip().c_str());

				for (mvAppItem* item : m_children)
				{
					// skip item if it's not shown
					if (!item->isShown())
						continue;

					// set item width
					if (item->getWidth() != 0)
						ImGui::SetNextItemWidth((float)item->getWidth());

					item->draw();

					item->getState().update();
					int a = 5;
				}
			}

			else
			{
				// Regular Tooltip (simple)
				if (!getTip().empty() && ImGui::IsItemHovered())
					ImGui::SetTooltip("%s", getTip().c_str());
			}

		}

		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "closable")) m_closable = ToBool(item);

			// helper for bit flipping
			auto flagop = [dict](const char* keyword, int flag, int& flags)
			{
				if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
			};

			// flags
			flagop("default_open",			ImGuiTreeNodeFlags_DefaultOpen,			m_flags);
			flagop("open_on_double_click",	ImGuiTreeNodeFlags_OpenOnDoubleClick,	m_flags);
			flagop("open_on_arrow",			ImGuiTreeNodeFlags_OpenOnArrow,			m_flags);
			flagop("leaf",					ImGuiTreeNodeFlags_Leaf,				m_flags);
			flagop("bullet",				ImGuiTreeNodeFlags_Bullet,				m_flags);


		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "closable", ToPyBool(m_closable));

			// helper to check and set bit
			auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
			{
				PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
			};

			// flags
			checkbitset("default_open",			ImGuiTreeNodeFlags_DefaultOpen,			m_flags);
			checkbitset("open_on_double_click", ImGuiTreeNodeFlags_OpenOnDoubleClick,	m_flags);
			checkbitset("open_on_arrow",		ImGuiTreeNodeFlags_OpenOnArrow,			m_flags);
			checkbitset("leaf",					ImGuiTreeNodeFlags_Leaf,				m_flags);
			checkbitset("bullet",				ImGuiTreeNodeFlags_Bullet,				m_flags);

		}

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

		mvTreeNode(const std::string& name)
			: mvBoolPtrBase(name, false, name)
		{
			m_description.container = true;
		}

		void draw() override
		{
			auto styleManager = m_styleManager.getScopedStyleManager();
			ScopedID id;
			
			ImGui::BeginGroup();
			if (ImGui::TreeNodeEx(m_label.c_str(), m_flags))
			{
				
				for (mvAppItem* item : m_children)
				{
					// skip item if it's not shown
					if (!item->isShown())
						continue;

					// set item width
					if (item->getWidth() != 0)
						ImGui::SetNextItemWidth((float)item->getWidth());

					item->draw();

					item->getState().update();
				}
				ImGui::TreePop();
			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			ImGui::EndGroup();
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;

			// helper for bit flipping
			auto flagop = [dict](const char* keyword, int flag, int& flags)
			{
				if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
			};

			// flags
			flagop("default_open",			ImGuiTreeNodeFlags_DefaultOpen,			m_flags);
			flagop("open_on_double_click",	ImGuiTreeNodeFlags_OpenOnDoubleClick,	m_flags);
			flagop("open_on_arrow",			ImGuiTreeNodeFlags_OpenOnArrow,			m_flags);
			flagop("leaf",					ImGuiTreeNodeFlags_Leaf,				m_flags);
			flagop("bullet",				ImGuiTreeNodeFlags_Bullet,				m_flags);
		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;

			// helper to check and set bit
			auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
			{
				PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
			};

			// flags
			checkbitset("default_open",			ImGuiTreeNodeFlags_DefaultOpen,			m_flags);
			checkbitset("open_on_double_click", ImGuiTreeNodeFlags_OpenOnDoubleClick,	m_flags);
			checkbitset("open_on_arrow",		ImGuiTreeNodeFlags_OpenOnArrow,			m_flags);
			checkbitset("leaf",					ImGuiTreeNodeFlags_Leaf,				m_flags);
			checkbitset("bullet",				ImGuiTreeNodeFlags_Bullet,				m_flags);

		}

	private:

		ImGuiTreeNodeFlags m_flags = ImGuiTreeNodeFlags_None;

	};

}