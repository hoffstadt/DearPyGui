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
	class mvChild : public mvBoolItemBase, public mvEventHandler
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Child, "add_child")

		mvChild(const std::string& name)
			: mvBoolItemBase(name, false), mvEventHandler()
		{
			m_container = true;
		}

		void draw() override
		{

			pushColorStyles();
			
			ImGui::BeginChild(m_label.c_str(), ImVec2(float(m_width), float(m_height)), m_border, 
				ImGuiWindowFlags_HorizontalScrollbar);

			for (mvAppItem* item : m_children)
			{
				// skip item if it's not shown
				if (!item->isShown())
					continue;

				// set item width
				if (item->getWidth() > 0)
					ImGui::SetNextItemWidth((float)item->getWidth());

				item->pushColorStyles();
				item->draw();
				item->popColorStyles();

				item->setHovered(ImGui::IsItemHovered());
				item->setActive(ImGui::IsItemActive());
				item->setFocused(ImGui::IsItemFocused());
				item->setClicked(ImGui::IsItemClicked());
				item->setVisible(ImGui::IsItemVisible());
				item->setEdited(ImGui::IsItemEdited());
				item->setActivated(ImGui::IsItemActivated());
				item->setDeactivated(ImGui::IsItemDeactivated());
				item->setDeactivatedAfterEdit(ImGui::IsItemDeactivatedAfterEdit());
				item->setToggledOpen(ImGui::IsItemToggledOpen());
				item->setRectMin({ ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y });
				item->setRectMax({ ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y });
				item->setRectSize({ ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y });
			}

			// TODO check if these work for child
			if (!m_tip .empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", m_tip.c_str());

			// allows this item to have a render callback
			//registerWindowFocusing();

			m_width = (int)ImGui::GetWindowWidth();
			m_height = (int)ImGui::GetWindowHeight();

			ImGui::EndChild();

			popColorStyles();
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "border")) m_border = ToBool(item);
		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "border", ToPyBool(m_border));
		}

	private:

		bool m_border = true;

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
			m_container = true;
		}

		void draw() override
		{
			pushColorStyles();

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
				if (item->getWidth() > 0)
					ImGui::SetNextItemWidth((float)item->getWidth());

				item->pushColorStyles();
				item->draw();
				item->popColorStyles();

				if (m_horizontal)
					ImGui::SameLine(0.0, m_hspacing);

				item->setHovered(ImGui::IsItemHovered());
				item->setActive(ImGui::IsItemActive());
				item->setFocused(ImGui::IsItemFocused());
				item->setClicked(ImGui::IsItemClicked());
				item->setVisible(ImGui::IsItemVisible());
				item->setEdited(ImGui::IsItemEdited());
				item->setActivated(ImGui::IsItemActivated());
				item->setDeactivated(ImGui::IsItemDeactivated());
				item->setDeactivatedAfterEdit(ImGui::IsItemDeactivatedAfterEdit());
				item->setToggledOpen(ImGui::IsItemToggledOpen());
				item->setRectMin({ ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y });
				item->setRectMax({ ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y });
				item->setRectSize({ ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y });
			}

			if (m_width != 0)
				ImGui::PopItemWidth();

			ImGui::EndGroup();

			popColorStyles();
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
	class mvCollapsingHeader : public mvBoolItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::CollapsingHeader, "add_collapsing_header")

		mvCollapsingHeader(const std::string& name)
			: mvBoolItemBase(name, true)
		{
			m_container = true;
		}

		void draw() override
		{
			pushColorStyles();

			bool* toggle = nullptr;
			if (m_closable)
				toggle = &m_value;

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
					if (item->getWidth() > 0)
						ImGui::SetNextItemWidth((float)item->getWidth());

					item->pushColorStyles();
					item->draw();
					item->popColorStyles();

					item->setHovered(ImGui::IsItemHovered());
					item->setActive(ImGui::IsItemActive());
					item->setFocused(ImGui::IsItemFocused());
					item->setClicked(ImGui::IsItemClicked());
					item->setVisible(ImGui::IsItemVisible());
					item->setEdited(ImGui::IsItemEdited());
					item->setActivated(ImGui::IsItemActivated());
					item->setDeactivated(ImGui::IsItemDeactivated());
					item->setDeactivatedAfterEdit(ImGui::IsItemDeactivatedAfterEdit());
					item->setToggledOpen(ImGui::IsItemToggledOpen());
					item->setRectMin({ ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y });
					item->setRectMax({ ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y });
					item->setRectSize({ ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y });
				}
			}

			else
			{
				// Regular Tooltip (simple)
				if (!getTip().empty() && ImGui::IsItemHovered())
					ImGui::SetTooltip("%s", getTip().c_str());
			}

			popColorStyles();
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
			//flagop("autosize", ImGuiWindowFlags_AlwaysAutoResize, m_windowflags);


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
			//checkbitset("autosize", ImGuiWindowFlags_AlwaysAutoResize, m_windowflags);

		}

	private:

		ImGuiTreeNodeFlags m_flags = 0;
		bool               m_closable = false;

	};

	//-----------------------------------------------------------------------------
	// mvTreeNode
	//-----------------------------------------------------------------------------
	class mvTreeNode : public mvBoolItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::TreeNode, "add_tree_node")

		mvTreeNode(const std::string& name)
			: mvBoolItemBase(name, false)
		{
			m_container = true;
		}

		void draw() override
		{
			pushColorStyles();

			ImGui::BeginGroup();
			if (ImGui::TreeNodeEx(m_label.c_str(), m_flags))
			{
				
				for (mvAppItem* item : m_children)
				{
					// skip item if it's not shown
					if (!item->isShown())
						continue;

					// set item width
					if (item->getWidth() > 0)
						ImGui::SetNextItemWidth((float)item->getWidth());

					item->pushColorStyles();
					item->draw();
					item->popColorStyles();

					item->setHovered(ImGui::IsItemHovered());
					item->setActive(ImGui::IsItemActive());
					item->setFocused(ImGui::IsItemFocused());
					item->setClicked(ImGui::IsItemClicked());
					item->setVisible(ImGui::IsItemVisible());
					item->setEdited(ImGui::IsItemEdited());
					item->setActivated(ImGui::IsItemActivated());
					item->setDeactivated(ImGui::IsItemDeactivated());
					item->setDeactivatedAfterEdit(ImGui::IsItemDeactivatedAfterEdit());
					item->setToggledOpen(ImGui::IsItemToggledOpen());
					item->setRectMin({ ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y });
					item->setRectMax({ ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y });
					item->setRectSize({ ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y });
				}

				ImGui::TreePop();
			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			ImGui::EndGroup();

			popColorStyles();
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
			flagop("default_open", ImGuiTreeNodeFlags_DefaultOpen, m_flags);


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
			checkbitset("default_open", ImGuiTreeNodeFlags_DefaultOpen, m_flags);

		}

	private:

		ImGuiTreeNodeFlags m_flags = 0;

	};

}