#include "mvTab.h"
#include "mvTabBar.h"
#include "mvApp.h"

namespace Marvel {
	void mvTab::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_tab", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "closable", "creates a button on the tab that can hide the tab", "False"},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::Bool, "no_reorder", "Disable reordering this tab or having another tab cross over this tab", "False"},
			{mvPythonDataType::Bool, "leading", "Enforce the tab position to the left of the tab bar (after the tab list popup button)", "False"},
			{mvPythonDataType::Bool, "trailing", "Enforce the tab position to the right of the tab bar (before the scrolling buttons)", "False"},
			{mvPythonDataType::Bool, "no_tooltip", "Disable tooltip for the given tab", "False"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
		}, "Adds a tab to a tab bar. Must be closed with the end command.", "None", "Containers") });
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
		mvImGuiThemeScope scope(this);

		// cast parent to mvTabBar
		auto parent = (mvTabBar*)m_parent;

		// check if this is first tab
		if (parent->getValue().empty())
		{
			// set mvTabBar value to the first tab name
			parent->setValue(m_core_config.name);
			*m_value = true;
		}

		// create tab item and see if it is selected
		if (ImGui::BeginTabItem(m_label.c_str(), m_closable ? &m_core_config.show : nullptr, m_flags))
		{

			// set other tab's value false
			for (auto child : parent->m_children)
			{
				if (child->getType() == mvAppItemType::TabItem)
					*((mvTab*)child.get())->m_value = false;
			}

			// set current tab value true
			*m_value = true;

			// run call back if it exists
			if (parent->getValue() != m_core_config.name)
				mvApp::GetApp()->getCallbackRegistry().addCallback(parent->getCallback(), m_core_config.name, parent->getCallbackData());

			parent->setValue(m_core_config.name);

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

			ImGui::EndTabItem();
		}

	}

	void mvTab::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
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

	PyObject* add_tab(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int closeable = false;
		const char* label = "";
		int show = true;
		int no_reorder = false;
		int leading = false;
		int trailing = false;
		int no_tooltip = false;
		const char* parent = "";
		const char* before = "";

		if (!(*mvApp::GetApp()->getParsers())["add_tab"].parse(args, kwargs, __FUNCTION__, &name, &closeable,
			&label, &show, &no_reorder, &leading, &trailing, &no_tooltip, &parent, &before))
			return ToPyBool(false);

		if (std::string(parent).empty())
		{
			auto parentItem = mvApp::GetApp()->getItemRegistry().topParent();

			if (parentItem == nullptr)
			{
				ThrowPythonException("add_tab must follow a call to add_tabbar.");
				return ToPyBool(false);
			}

			else if (parentItem->getType() == mvAppItemType::TabBar)
			{
				auto item = CreateRef<mvTab>(name);
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

			else
				ThrowPythonException("add_tab was called incorrectly. Did you forget to call end_tab?");
		}

		else
		{
			auto parentItem = mvApp::GetApp()->getItemRegistry().getItem(parent);

			if (parentItem == nullptr)
			{
				ThrowPythonException("add_tab parent must exist.");
				return ToPyBool(false);
			}

			else if (parentItem->getType() == mvAppItemType::TabBar)
			{
				auto item = CreateRef<mvTab>(name);
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

			else
			{
				ThrowPythonException("add_tab parent must be a tab bar.");
				return ToPyBool(false);
			}
		}

		return ToPyBool(false);
	}

}