#include <utility>
#include "mvSelectable.h"
#include "mvApp.h"
#include "mvValueStorage.h"
#include "mvPythonTranslator.h"
#include "mvGlobalIntepreterLock.h"

namespace Marvel {

	mvSelectable::mvSelectable(const std::string& name, bool default_value, const std::string& dataSource)
		: mvBoolPtrBase(name, default_value, dataSource)
	{
		m_description.disableAllowed = true;
	}

	void mvSelectable::setEnabled(bool value)
	{
		if (value)
			m_flags &= ~ImGuiSelectableFlags_Disabled;

		else
			m_flags |= ImGuiSelectableFlags_Disabled;

		m_enabled = value;
	}

	void mvSelectable::draw()
	{
		auto styleManager = m_styleManager.getScopedStyleManager();
		ScopedID id;

		if (ImGui::Selectable(m_label.c_str(), m_value, m_flags))
		{

			mvApp::GetApp()->getCallbackRegistry().addCallback(m_callback, m_name, m_callbackData);

			// Context Menu
			if (!m_popup.empty())
				ImGui::OpenPopup(m_popup.c_str());
		}

	}

	void mvSelectable::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags, bool flip)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// window flags
		flagop("span_columns", ImGuiSelectableFlags_SpanAllColumns, m_flags, false);

	}

	void mvSelectable::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags, bool flip)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		// window flags
		checkbitset("span_columns", ImGuiSelectableFlags_SpanAllColumns, m_flags, false);
	}

}