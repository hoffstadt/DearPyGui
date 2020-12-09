#include <utility>
#include "mvButton.h"
#include "mvApp.h"
#include "mvValueStorage.h"
#include "mvPythonTranslator.h"
#include "mvGlobalIntepreterLock.h"

namespace Marvel {

	mvButton::mvButton(const std::string& name)
		: mvAppItem(name)
	{
		m_description.disableAllowed = true;
	}

	void mvButton::draw()
	{
		auto styleManager = m_styleManager.getScopedStyleManager();
		ScopedID id;

		if (!m_enabled)
		{
			ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
			disabled_color.w = 0.392f;
			styleManager.addColorStyle(ImGuiCol_Button, disabled_color);
			styleManager.addColorStyle(ImGuiCol_ButtonHovered, disabled_color);
			styleManager.addColorStyle(ImGuiCol_ButtonActive, disabled_color);
			styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
		}

		if (m_small)
		{
			if (ImGui::SmallButton(m_label.c_str()))
				mvCallbackRegistry::GetCallbackRegistry()->addCallback(getCallback(false), m_name, m_callbackData);

			return;
		}

		if (m_arrow)
		{
			if (ImGui::ArrowButton(m_label.c_str(), m_direction))
				mvCallbackRegistry::GetCallbackRegistry()->addCallback(getCallback(false), m_name, m_callbackData);

			return;
		}

		if (ImGui::Button(m_label.c_str(), ImVec2((float)m_width, (float)m_height)))
			mvCallbackRegistry::GetCallbackRegistry()->addCallback(getCallback(false), m_name, m_callbackData);

	}

	void mvButton::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;

		if (PyObject* item = PyDict_GetItemString(dict, "small")) m_small = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "arrow")) m_arrow = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "direction")) m_direction = ToInt(item);
	}

	void mvButton::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		PyDict_SetItemString(dict, "small", ToPyBool(m_small));
		PyDict_SetItemString(dict, "arrow", ToPyBool(m_arrow));
		PyDict_SetItemString(dict, "direction", ToPyInt(m_direction));
	}

}