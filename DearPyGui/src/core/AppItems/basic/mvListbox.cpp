#include <utility>
#include "mvListbox.h"
#include "mvApp.h"
#include "mvValueStorage.h"
#include "mvPythonTranslator.h"
#include "mvGlobalIntepreterLock.h"

namespace Marvel {

	mvListbox::mvListbox(const std::string& name, int default_value, const std::string& dataSource)
		: mvIntPtrBase(name, default_value, dataSource)
	{
		m_description.disableAllowed = true;
	}

	void mvListbox::draw()
	{
		auto styleManager = m_styleManager.getScopedStyleManager();
		ScopedID id;

		if (!m_enabled)
		{
			ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
			disabled_color.w = 0.392f;
			styleManager.addColorStyle(ImGuiCol_Header, disabled_color);
			styleManager.addColorStyle(ImGuiCol_HeaderHovered, disabled_color);
			styleManager.addColorStyle(ImGuiCol_HeaderActive, disabled_color);
			styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
			styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
			styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
			styleManager.addColorStyle(ImGuiCol_ScrollbarBg, disabled_color);
			styleManager.addColorStyle(ImGuiCol_ScrollbarGrab, disabled_color);
			styleManager.addColorStyle(ImGuiCol_ScrollbarGrabHovered, disabled_color);
			styleManager.addColorStyle(ImGuiCol_ScrollbarGrabActive, disabled_color);
			styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
			m_disabled_value = *m_value;
		}

		if (ImGui::ListBox(m_label.c_str(), m_enabled ? m_value : &m_disabled_value, m_charNames.data(), (int)m_names.size(), m_itemsHeight))
			mvCallbackRegistry::GetCallbackRegistry()->addCallback(getCallback(false), m_name, m_callbackData);

	}

	void mvListbox::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		if (PyObject* item = PyDict_GetItemString(dict, "num_items")) m_itemsHeight = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "items"))
		{
			m_names = ToStringVect(item);
			m_charNames.clear();
			for (const std::string& item : m_names)
				m_charNames.emplace_back(item.c_str());
		}
	}

	void mvListbox::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		PyDict_SetItemString(dict, "items", ToPyList(m_names));
		PyDict_SetItemString(dict, "num_items", ToPyInt(m_itemsHeight));
	}

}