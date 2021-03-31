#include "mvTabBar.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvTab.h"
#include "mvFontScope.h"

namespace Marvel {

	void mvTabBar::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_tab_bar", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "reorderable", "allows for moveable tabs", "False"},
			{mvPythonDataType::Callable, "callback", "Registers a callback", "None"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::String, "parent", "Parent to add this item to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
		}, "Adds a tab bar.", "None", "Containers") });
	}

	mvTabBar::mvTabBar(const std::string& name)
		:
		mvStringPtrBase(name, "")
	{
		m_description.container = true;
	}

	std::string& mvTabBar::getSpecificValue()
	{
		return m_uiValue;
	}

	void mvTabBar::setValue(const std::string& value)
	{
		m_uiValue = value;
	}

	void mvTabBar::draw()
	{
		ScopedID id;
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);
		ImGui::BeginGroup();

		if (ImGui::BeginTabBar(m_label.c_str(), m_flags))
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

				if (*m_value == item->getCoreConfig().name && m_lastValue != *m_value)
					static_cast<mvTab*>(item.get())->addFlag(ImGuiTabItemFlags_SetSelected);

				item->draw();

				if (*m_value == item->getCoreConfig().name)
					static_cast<mvTab*>(item.get())->removeFlag(ImGuiTabItemFlags_SetSelected);

				item->getState().update();
			}

			ImGui::EndTabBar();
		}

		ImGui::EndGroup();

		*m_value = m_uiValue;
		m_lastValue = *m_value;
	}

	void mvTabBar::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 

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
		 

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		// window flags
		checkbitset("reorderable", ImGuiTabBarFlags_Reorderable, m_flags);
	}

	PyObject* add_tab_bar(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		int reorderable = false;
		PyObject* callback = nullptr;
		PyObject* callback_data = nullptr;
		int show = true;
		const char* parent = "";
		const char* before = "";

		if (!(mvApp::GetApp()->getParsers())["add_tab_bar"].parse(args, kwargs, __FUNCTION__, &name, &reorderable,
			&callback, &callback_data, &show, &parent, &before))
			return ToPyBool(false);

		auto item = CreateRef<mvTabBar>(name);

		if (callback)
			Py_XINCREF(callback);

		item->setCallback(callback);

		if (callback_data)
			Py_XINCREF(callback_data);

		item->setCallbackData(callback_data);
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

}