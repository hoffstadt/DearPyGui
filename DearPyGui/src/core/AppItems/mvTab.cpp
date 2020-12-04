#include "mvTab.h"
#include "mvApp.h"
#include "PythonUtilities/mvPythonTranslator.h"

namespace Marvel {

	mvTabBar::mvTabBar(const std::string& name)
		: 
		mvStringPtrBase(name, "", name)
	{
		m_description.container = true;
	}

	std::string& mvTabBar::getValue() 
	{ 
		return *m_value; 
	}

	void mvTabBar::setValue(const std::string& value) 
	{ 
		*m_value = value; 
	}

	void mvTabBar::draw()
	{
		auto styleManager = m_styleManager.getScopedStyleManager();
		ScopedID id;
		ImGui::BeginGroup();

		if (ImGui::BeginTabBar(m_label.c_str(), m_flags))
		{
			for (mvAppItem* item : m_children)
			{
				// skip item if it's not shown
				if (!item->isShown())
					continue;

				// set item width
				if (item->isSizeDirty())
				{
					ImGui::SetNextItemWidth((float)item->getWidth());
					item->setSizeClean();
				}

				item->draw();

				// Regular Tooltip (simple)
				if (!item->getTip().empty() && ImGui::IsItemHovered())
					ImGui::SetTooltip("%s", item->getTip().c_str());

				item->getState().update();
			}

			ImGui::EndTabBar();
		}

		ImGui::EndGroup();
	}

	void mvTabBar::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// window flags
		flagop("reorderable", ImGuiTabBarFlags_Reorderable, m_flags);

	}

	void mvTabBar::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		// window flags
		checkbitset("reorderable", ImGuiTabBarFlags_Reorderable, m_flags);
	}

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
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			// set other tab's value false
			for (mvAppItem* child : parent->getChildren())
			{
				if (child->getType() == mvAppItemType::TabItem)
					*((mvTab*)child)->m_value = false;
			}

			// set current tab value true
			*m_value = true;

			// run call back if it exists
			if (parent->getValue() != m_name)
			{
				mvApp::GetApp()->addCallback(parent->getCallback(), m_name, parent->getCallbackData());

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());
			}

			parent->setValue(m_name);

			for (mvAppItem* item : m_children)
			{
				// skip item if it's not shown
				if (!item->isShown())
					continue;

				// set item width
				if (item->isSizeDirty())
				{
					ImGui::SetNextItemWidth((float)item->getWidth());
					item->setSizeClean();
				}

				item->draw();

				// Regular Tooltip (simple)
				if (!item->getTip().empty() && ImGui::IsItemHovered())
					ImGui::SetTooltip("%s", item->getTip().c_str());

				item->getState().update();
			}

			ImGui::EndTabItem();
		}

		else
		{
			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());
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

	mvTabButton::mvTabButton(const std::string& name)
		: 
		mvAppItem(name)
	{
	}

	void mvTabButton::draw()
	{
		auto styleManager = m_styleManager.getScopedStyleManager();
		ScopedID id;

		if (ImGui::TabItemButton(m_label.c_str(), m_flags))
			mvApp::GetApp()->addCallback(getCallback(false), m_name, m_callbackData);

	}

	void mvTabButton::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;

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

	void mvTabButton::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;

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