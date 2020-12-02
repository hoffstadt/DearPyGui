#include "mvText.h"
#include "PythonUtilities/mvPythonTranslator.h"

namespace Marvel {

	static std::string FindRenderedTextEnd(const char* text, const char* text_end = nullptr)
	{
		int size = 0;

		const char* text_display_end = text;
		if (!text_end)
			text_end = (const char*)-1;

		while (text_display_end < text_end && *text_display_end != '\0' && (text_display_end[0] != '#' || text_display_end[1] != '#'))
		{
			text_display_end++;
			size++;
		}

		char* cvalue = new char[size + 1];
		for (int i = 0; i < size; i++)
			cvalue[i] = text[i];

		cvalue[size] = '\0';

		return std::string(cvalue);
	}

	mvText::mvText(const std::string& name, const std::string& default_value, const std::string& dataSource)
		:
		mvPtrBase<std::string, 1>(name, default_value.empty() ? name : default_value, dataSource)
	{
		m_description.duplicatesAllowed = true;
	}

	void mvText::draw()
	{

		if (m_color.specified)
			ImGui::PushStyleColor(ImGuiCol_Text, m_color);

		if (m_wrap >= 0)
			ImGui::PushTextWrapPos((float)m_wrap);

		if (m_bullet)
			ImGui::Bullet();

		//ImGui::Text("%s", m_value.c_str());
		ImGui::TextUnformatted(m_value->c_str()); // this doesn't have a buffer size limit

		if (m_wrap >= 0)
			ImGui::PopTextWrapPos();

		if (m_color.specified)
			ImGui::PopStyleColor();

	}

	void mvText::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
		if (PyObject* item = PyDict_GetItemString(dict, "wrap")) m_wrap = ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "bullet")) m_bullet = ToBool(item);

	}

	void mvText::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		PyDict_SetItemString(dict, "color", ToPyColor(m_color));
		PyDict_SetItemString(dict, "wrap", ToPyInt(m_wrap));
		PyDict_SetItemString(dict, "bullet", ToPyBool(m_bullet));
	}

	mvLabelText::mvLabelText(const std::string& name, const std::string& value, const std::string& dataSource)
		:
		mvPtrBase<std::string, 1>(name, value, dataSource)
	{
		m_label = FindRenderedTextEnd(m_name.c_str());
	}

	void mvLabelText::draw()
	{

		if (m_color.specified)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, m_color);

			ImGui::TextUnformatted(m_value->c_str());

			ImGui::PopStyleColor();

			ImGui::SameLine();

			auto styleManager = m_styleManager.getScopedStyleManager();
			ImGui::TextUnformatted(m_label.c_str());
		}

		else
			ImGui::LabelText(m_label.c_str(), m_value->c_str());

	}

	void mvLabelText::setExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
	}

	void mvLabelText::getExtraConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		mvGlobalIntepreterLock gil;
		PyDict_SetItemString(dict, "color", ToPyColor(m_color));
	}
}