#include <utility>
#include "mvCheckbox.h"
#include "mvApp.h"
#include "mvValueStorage.h"
#include "mvPythonTranslator.h"
#include "mvGlobalIntepreterLock.h"

namespace Marvel {

	mvCheckbox::mvCheckbox(const std::string& name, bool default_value, const std::string& dataSource)
		: mvBoolPtrBase(name, default_value, dataSource)
	{
		m_description.disableAllowed = true;
	}

	void mvCheckbox::draw()
	{
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

		if (ImGui::Checkbox(m_label.c_str(), m_enabled ? m_value : &m_disabled_value))
			mvApp::GetApp()->getCallbackRegistry().addCallback(getCallback(false), m_name, m_callbackData);

	}

}