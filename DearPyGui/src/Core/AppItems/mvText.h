#pragma once

#include "Core/AppItems/mvTypeBases.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvText
//     * mvLabelText
//
//-----------------------------------------------------------------------------

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

		char* cvalue = new char[size+1];
		for (int i = 0; i < size; i++)
			cvalue[i] = text[i];

		cvalue[size] = '\0';

		return std::string(cvalue);
	}

	//-----------------------------------------------------------------------------
	// mvText
	//-----------------------------------------------------------------------------
	class mvText : public mvStringPtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Text, "add_text")

		mvText(const std::string& name, const std::string& default_value, const std::string& dataSource)
			: mvStringPtrBase(name, default_value.empty() ? name : default_value, dataSource)
		{
		}

		[[nodiscard]] bool areDuplicatesAllowed() const override { return true; }

		void draw() override
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

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
			if (PyObject* item = PyDict_GetItemString(dict, "wrap")) m_wrap = ToInt(item);
			if (PyObject* item = PyDict_GetItemString(dict, "bullet")) m_bullet = ToBool(item);

		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "color", ToPyColor(m_color));
			PyDict_SetItemString(dict, "wrap", ToPyInt(m_wrap));
			PyDict_SetItemString(dict, "bullet", ToPyBool(m_bullet));
		}

	private:

		mvColor m_color = {1000, 0, 0, 255, false};
		int     m_wrap = 0;
		bool    m_bullet = false;

	};

	//-----------------------------------------------------------------------------
	// mvLabelText
	//-----------------------------------------------------------------------------
	class mvLabelText : public mvStringPtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::LabelText, "add_label_text")

			mvLabelText(const std::string& name, const std::string& value, const std::string& dataSource)
			: mvStringPtrBase(name, value, dataSource)
		{
			m_label = FindRenderedTextEnd(m_name.c_str());
		}

		void draw() override
		{

			if (m_color.specified)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, m_color);

				ImGui::TextUnformatted(m_value->c_str());

				ImGui::PopStyleColor();

				ImGui::SameLine();

				pushColorStyles();
				ImGui::TextUnformatted(m_label.c_str());
				popColorStyles();
			}

			else
				ImGui::LabelText(m_label.c_str(), m_value->c_str());

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

			
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item);
		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "color", ToPyColor(m_color));
		}

	private:

		mvColor m_color = { 1000, 0, 0, 255, false };

	};

}