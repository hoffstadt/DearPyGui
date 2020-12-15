#include <utility>
#include "mvCombo.h"
#include "mvApp.h"
#include "mvValueStorage.h"
#include "mvPythonTranslator.h"
#include "mvGlobalIntepreterLock.h"

namespace Marvel {

	mvCombo::mvCombo(const std::string& name, const std::string& default_value, const std::string& dataSource)
		: mvStringPtrBase(name, default_value, dataSource)
	{
		m_description.disableAllowed = true;
	}

	void mvCombo::draw()
	{

		auto styleManager = m_styleManager.getScopedStyleManager();
		ScopedID id;

		static std::vector<std::string> disabled_items{};
		if (!m_enabled)
		{
			ImVec4 disabled_color = ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
			disabled_color.w = 0.392f;
			styleManager.addColorStyle(ImGuiCol_FrameBg, disabled_color);
			styleManager.addColorStyle(ImGuiCol_FrameBgHovered, disabled_color);
			styleManager.addColorStyle(ImGuiCol_FrameBgActive, disabled_color);
			styleManager.addColorStyle(ImGuiCol_Button, disabled_color);
			styleManager.addColorStyle(ImGuiCol_ButtonHovered, disabled_color);
			styleManager.addColorStyle(ImGuiCol_ButtonActive, disabled_color);
			styleManager.addColorStyle(ImGuiCol_PopupBg, { 0.0f, 0.0f, 0.0f, 0.0f });
			styleManager.addColorStyle(ImGuiCol_Border, { 0.0f, 0.0f, 0.0f, 0.0f });
			styleManager.addColorStyle(ImGuiCol_Text, ImVec4(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled)));
		}
		if (ImGui::BeginCombo(m_label.c_str(), m_value->c_str(), m_flags)) // The second parameter is the label previewed before opening the combo.
		{
			for (const auto& name : m_enabled ? m_items : disabled_items)
			{
				bool is_selected = (*m_value == name);
				if (ImGui::Selectable((name).c_str(), is_selected))
				{
					if (m_enabled) { *m_value = name; }
					mvApp::GetApp()->getCallbackRegistry().addCallback(m_callback, m_name, m_callbackData);

				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
			}

			ImGui::EndCombo();
		}

	}

	void mvCombo::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		if (PyObject* item = PyDict_GetItemString(dict, "items")) m_items = ToStringVect(item);

		// helpers for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		auto conflictingflagop = [dict](const std::vector<std::string>& keywords, std::vector<int> flags, int& mflags)
		{

			for (size_t i = 0; i < keywords.size(); i++)
			{
				if (PyObject* item = PyDict_GetItemString(dict, keywords[i].c_str()))
				{
					//turning all conflicting flags false
					for (const auto& flag : flags) mflags &= ~flag;
					//writing only the first conflicting flag
					ToBool(item) ? mflags |= flags[i] : mflags &= ~flags[i];
					break;
				}
			}

		};

		flagop("popup_align_left", ImGuiComboFlags_PopupAlignLeft, m_flags);
		flagop("no_arrow_button", ImGuiComboFlags_NoArrowButton, m_flags);
		flagop("no_preview", ImGuiComboFlags_NoPreview, m_flags);

		std::vector<std::string> HeightKeywords{
			"height_small",
			"height_regular",
			"height_large",
			"height_largest" };
		std::vector<int> HeightFlags{
			ImGuiComboFlags_HeightSmall,
			ImGuiComboFlags_HeightRegular,
			ImGuiComboFlags_HeightLarge,
			ImGuiComboFlags_HeightLargest };

		conflictingflagop(HeightKeywords, HeightFlags, m_flags);

	}

	void mvCombo::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		PyDict_SetItemString(dict, "items", ToPyList(m_items));

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};
		checkbitset("popup_align_left", ImGuiComboFlags_PopupAlignLeft, m_flags);
		checkbitset("height_small", ImGuiComboFlags_HeightSmall, m_flags);
		checkbitset("height_regular", ImGuiComboFlags_HeightRegular, m_flags);
		checkbitset("height_large", ImGuiComboFlags_HeightLarge, m_flags);
		checkbitset("height_largest", ImGuiComboFlags_HeightLargest, m_flags);
		checkbitset("no_arrow_button", ImGuiComboFlags_NoArrowButton, m_flags);
		checkbitset("no_preview", ImGuiComboFlags_NoPreview, m_flags);
	}

}