#include "mvTabButton.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvTabButton::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_tab_button", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::Bool, "no_reorder", "Disable reordering this tab or having another tab cross over this tab", "False"},
			{mvPythonDataType::Bool, "leading", "Enforce the tab position to the left of the tab bar (after the tab list popup button)", "False"},
			{mvPythonDataType::Bool, "trailing", "Enforce the tab position to the right of the tab bar (before the scrolling buttons)", "False"},
			{mvPythonDataType::Bool, "no_tooltip", "Disable tooltip for the given tab", "False"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
		}, "Adds a tab button to a tab bar", "None", "Containers") });
	}

	mvTabButton::mvTabButton(const std::string& name)
		:
		mvAppItem(name)
	{
	}

	void mvTabButton::draw()
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		if (ImGui::TabItemButton(m_label.c_str(), m_flags))
			mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_core_config.name, m_core_config.callback_data);

	}

	void mvTabButton::updateConfig(mvAppItemConfig* config)
	{
		auto aconfig = (mvTabButtonConfig*)config;

		m_core_config.label = config->label;
		m_core_config.show = config->show;
		m_core_config.callback = config->callback;
		m_core_config.callback_data = config->callback_data;

		m_config.source = aconfig->source;

		if (config != &m_config)
			m_config = *aconfig;
	}

	mvAppItemConfig* mvTabButton::getConfig()
	{
		return &m_config;
	}

	void mvTabButton::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 

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

	PyObject* add_tab_button(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* label = "";
		int show = true;
		int no_reorder = false;
		int leading = false;
		int trailing = false;
		int no_tooltip = false;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		const char* parent = "";
		const char* before = "";

		if (!(mvApp::GetApp()->getParsers())["add_tab_button"].parse(args, kwargs, __FUNCTION__, &name,
			&label, &show, &no_reorder, &leading, &trailing, &no_tooltip, &callback,
			&callback_data, &parent, &before))
			return ToPyBool(false);

		if (std::string(parent).empty())
		{
			auto parentItem = mvApp::GetApp()->getItemRegistry().topParent();

			if (parentItem == nullptr)
			{
				ThrowPythonException("add_tab_button must follow a call to add_tabbar.");
				return ToPyBool(false);
			}

			else if (parentItem->getType() == mvAppItemType::mvTabBar)
			{
				auto item = CreateRef<mvTabButton>(name);
				if (callback)
					Py_XINCREF(callback);
				item->setCallback(callback);
				if (callback_data)
					Py_XINCREF(callback_data);
				item->setCallbackData(callback_data);

				item->checkConfigDict(kwargs);
				item->setConfigDict(kwargs);
				item->setExtraConfigDict(kwargs);

				mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before);

				return GetPyNone();

			}

			else
				ThrowPythonException("add_tab_bar was called incorrectly. Did you forget to call end_tab?");
		}

		else
		{
			auto parentItem = mvApp::GetApp()->getItemRegistry().getItem(parent);

			if (parentItem == nullptr)
			{
				ThrowPythonException("add_tab parent must exist.");
				return ToPyBool(false);
			}

			else if (parentItem->getType() == mvAppItemType::mvTabBar)
			{
				auto item = CreateRef<mvTabButton>(name);
				if (callback)
					Py_XINCREF(callback);
				item->setCallback(callback);
				if (callback_data)
					Py_XINCREF(callback_data);
				item->setCallbackData(callback_data);
				item->checkConfigDict(kwargs);
				item->setConfigDict(kwargs);
				item->setExtraConfigDict(kwargs);

				mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before);

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