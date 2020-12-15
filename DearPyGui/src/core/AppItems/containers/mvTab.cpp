#include "mvTab.h"
#include "mvTabBar.h"
#include "mvApp.h"
#include "mvPythonTranslator.h"
#include "mvGlobalIntepreterLock.h"

namespace Marvel {

	mvTab::mvTab(const std::string& name)
		: 
		mvBoolPtrBase(name, false, name)
	{
		m_description.container = true;
	}

	void mvTab::draw()
	{
		auto styleManager = m_styleManager.getScopedStyleManager();
		ScopedID id;

		// cast parent to mvTabBar
		auto parent = (mvTabBar*)m_parent;

		// check if this is first tab
		if (parent->getValue().empty())
		{
			// set mvTabBar value to the first tab name
			parent->setValue(m_name);
			*m_value = true;
		}

		// create tab item and see if it is selected
		if (ImGui::BeginTabItem(m_label.c_str(), m_closable ? &m_show : nullptr, m_flags))
		{
			// Regular Tooltip (simple)
			if (!m_tip.empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", m_tip.c_str());

			// set other tab's value false
			for (auto child : parent->m_children)
			{
				if (child->getType() == mvAppItemType::TabItem)
					*((mvTab*)child.get())->m_value = false;
			}

			// set current tab value true
			*m_value = true;

			// run call back if it exists
			if (parent->getValue() != m_name)
			{
				mvCallbackRegistry::GetCallbackRegistry()->addCallback(parent->getCallback(), m_name, parent->getCallbackData());

				// Context Menu
				if (!m_popup.empty())
					ImGui::OpenPopup(m_popup.c_str());
			}

			parent->setValue(m_name);

			for (auto& item : m_children)
			{
				// skip item if it's not shown
				if (!item->m_show)
					continue;

				// set item width
				if (item->m_width != 0)
					ImGui::SetNextItemWidth((float)item->m_width);

				item->draw();

				// Regular Tooltip (simple)
				if (!item->m_tip.empty() && ImGui::IsItemHovered())
					ImGui::SetTooltip("%s", item->m_tip.c_str());

				item->getState().update();
			}

			ImGui::EndTabItem();
		}

		else
		{
			// Regular Tooltip (simple)
			if (!m_tip.empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", m_tip.c_str());
		}

	}

	void mvTab::setExtraConfigDict(PyObject* dict)
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

		// window flags
		flagop("no_reorder", ImGuiTabItemFlags_NoReorder, m_flags);
		flagop("leading", ImGuiTabItemFlags_Leading, m_flags);
		flagop("trailing", ImGuiTabItemFlags_Trailing, m_flags);
		flagop("no_tooltip", ImGuiTabItemFlags_NoTooltip, m_flags);

		if (m_flags & ImGuiTabItemFlags_Leading && m_flags & ImGuiTabItemFlags_Trailing)
			m_flags &= ~ImGuiTabItemFlags_Leading;

	}

	void mvTab::getExtraConfigDict(PyObject* dict)
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

		// window flags
		checkbitset("no_reorder", ImGuiTabBarFlags_Reorderable, m_flags);
		checkbitset("leading", ImGuiTabItemFlags_Leading, m_flags);
		checkbitset("trailing", ImGuiTabItemFlags_Trailing, m_flags);
		checkbitset("no_tooltip", ImGuiTabItemFlags_NoTooltip, m_flags);

	}

}