#pragma once

#include "mvMenu.h"
#include "mvApp.h"
#include "mvPythonTranslator.h"
#include "mvGlobalIntepreterLock.h"

namespace Marvel {


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
			for (mvAppItem* sibling : m_parent->getChildren())
			{
				// ensure sibling
				if (sibling->getType() == mvAppItemType::Menu)
					*((mvMenu*)sibling)->m_value = false;
			}

			// set current menu value true
			*m_value = true;

			for (mvAppItem* item : m_children)
			{
				// skip item if it's not shown
				if (!item->isShown())
					continue;

				// set item width
				if (item->getWidth() != 0)
					ImGui::SetNextItemWidth((float)item->getWidth());

				item->draw();

				// Regular Tooltip (simple)
				if (!item->getTip().empty() && ImGui::IsItemHovered())
					ImGui::SetTooltip("%s", item->getTip().c_str());

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

}