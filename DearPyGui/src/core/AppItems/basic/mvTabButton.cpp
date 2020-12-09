#include "mvTabButton.h"
#include "mvApp.h"
#include "mvPythonTranslator.h"
#include "mvGlobalIntepreterLock.h"

namespace Marvel {

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
			mvCallbackRegistry::GetCallbackRegistry()->addCallback(getCallback(false), m_name, m_callbackData);

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