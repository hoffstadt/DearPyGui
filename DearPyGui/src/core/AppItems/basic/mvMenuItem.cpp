#pragma once

#include "mvMenuItem.h"
#include "mvPythonTranslator.h"
#include "mvApp.h"
#include "mvGlobalIntepreterLock.h"

namespace Marvel {


	mvMenuItem::mvMenuItem(const std::string& name)
		: mvBoolPtrBase(name, false, name) {}

	void mvMenuItem::draw()
	{
		auto styleManager = m_styleManager.getScopedStyleManager();
		ScopedID id;

		// create menuitem and see if its selected
		if (ImGui::MenuItem(m_label.c_str(), m_shortcut.c_str(), m_check ? m_value : nullptr, m_enabled))
		{

			// set other menuitems's value false on same level
			for (mvAppItem* sibling : m_parent->m_children)
			{
				// ensure sibling
				if (sibling->getType() == mvAppItemType::MenuItem)
					*((mvMenuItem*)sibling)->m_value = false;
			}

			*m_value = true;

			mvCallbackRegistry::GetCallbackRegistry()->addCallback(m_callback, m_name, m_callbackData);

		}

	}

	void mvMenuItem::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		if (PyObject* item = PyDict_GetItemString(dict, "shortcut")) m_shortcut = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "check")) m_check = ToBool(item);

	}

	void mvMenuItem::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		PyDict_SetItemString(dict, "shortcut", ToPyString(m_shortcut));
		PyDict_SetItemString(dict, "check", ToPyBool(m_check));
	}

}