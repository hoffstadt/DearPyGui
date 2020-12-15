#include <utility>
#include "mvRadioButton.h"
#include "mvApp.h"
#include "mvValueStorage.h"
#include "mvPythonTranslator.h"
#include "mvGlobalIntepreterLock.h"

namespace Marvel {

	mvRadioButton::mvRadioButton(const std::string& name, int default_value, const std::string& dataSource)
		: mvIntPtrBase(name, default_value, dataSource)
	{
		m_description.disableAllowed = true;
	}

	void mvRadioButton::draw()
	{

		ImGui::BeginGroup();

		auto styleManager = m_styleManager.getScopedStyleManager();
		ScopedID id;

		if (!m_enabled)
		{
			ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
			disabled_color.w = 0.392f;
			styleManager.addColorStyle(ImGuiCol_CheckMark, disabled_color);
			styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
			styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
			styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
			styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
			m_disabled_value = *m_value;
		}
		for (size_t i = 0; i < m_itemnames.size(); i++)
		{
			if (m_horizontal && i != 0)
				ImGui::SameLine();

			if (ImGui::RadioButton((m_itemnames[i] + "##" + m_name).c_str(), m_enabled ? m_value : &m_disabled_value, (int)i))
				mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callbackData);
		}

		ImGui::EndGroup();
	}

	void mvRadioButton::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		if (PyObject* item = PyDict_GetItemString(dict, "items")) m_itemnames = ToStringVect(item);
		if (PyObject* item = PyDict_GetItemString(dict, "horizontal")) m_horizontal = ToBool(item);
	}

	void mvRadioButton::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		PyDict_SetItemString(dict, "items", ToPyList(m_itemnames));
		PyDict_SetItemString(dict, "horizontal", ToPyBool(m_horizontal));
	}

}