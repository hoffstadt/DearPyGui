#pragma once

#include "mvMenu.h"
#include "mvApp.h"
#include "mvPythonTranslator.h"
#include "mvGlobalIntepreterLock.h"

namespace Marvel {

	void mvMenu::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "add_menu", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "label", "", "''"},
			{mvPythonDataType::Bool, "show", "Attempt to render", "True"},
			{mvPythonDataType::String, "parent", "Parent this item will be added to. (runtime adding)", "''"},
			{mvPythonDataType::String, "before", "This item will be displayed before the specified item in the parent. (runtime adding)", "''"},
			{mvPythonDataType::Bool, "enabled", "", "True"},
		}, "Adds a menu to an existing menu bar. Must be followed by a call to end.", "None", "Containers") });
	}

	mvMenu::mvMenu(const std::string& name)
			: mvBoolPtrBase(name, false, name)
	{
		m_description.container = true;
	}

	void mvMenu::draw()
	{
		auto styleManager = m_styleManager.getScopedStyleManager();
		ScopedID id;

		// create menu and see if its selected
		if (ImGui::BeginMenu(m_label.c_str(), m_enabled))
		{

			// set other menus's value false on same level
			for (auto sibling : m_parent->m_children)
			{
				// ensure sibling
				if (sibling->getType() == mvAppItemType::Menu)
					*((mvMenu*)sibling.get())->m_value = false;
			}

			// set current menu value true
			*m_value = true;

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

			registerWindowFocusing();

			ImGui::EndMenu();
		}

	}

	void mvMenu::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		if (PyObject* item = PyDict_GetItemString(dict, "enabled")) m_enabled = ToBool(item);

	}

	void mvMenu::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		PyDict_SetItemString(dict, "enabled", ToPyBool(m_enabled));
	}


	PyObject* add_menu(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		const char* label = "";
		int show = true;
		const char* parent = "";
		const char* before = "";
		int enabled = true;

		if (!(*mvApp::GetApp()->getParsers())["add_menu"].parse(args, kwargs, __FUNCTION__, &name,
			&label, &show, &parent, &before, &enabled))
			return ToPyBool(false);

		auto item = CreateRef<mvMenu>(name);

		item->checkConfigDict(kwargs);
		item->setConfigDict(kwargs);
		item->setExtraConfigDict(kwargs);

		if (mvApp::GetApp()->getItemRegistry().addItemWithRuntimeChecks(item, parent, before))
		{
			mvApp::GetApp()->getItemRegistry().pushParent(item);
			return ToPyBool(true);
		}
		return ToPyBool(false);
	}

}