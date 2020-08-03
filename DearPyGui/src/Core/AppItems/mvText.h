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
	class mvText : public mvStringItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Text)

		mvText(const std::string& parent, const std::string& name, int wrap, mvColor color, bool bullet)
			: mvStringItemBase(parent, name, name), m_wrap(wrap), m_color(color), m_bullet(bullet)
		{
		}

		virtual bool areDuplicatesAllowed() const override { return true; }

		virtual void draw() override
		{

			if (m_color.specified)
				ImGui::PushStyleColor(ImGuiCol_Text, m_color);

			if (m_wrap != 0)
				ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + m_wrap);

			if (m_bullet)
				ImGui::Bullet();

			ImGui::Text("%s", m_value.c_str());

			if (m_wrap != 0)
				ImGui::PopTextWrapPos();

			if (m_color.specified)
				ImGui::PopStyleColor();
		}

	private:

		mvColor m_color;
		int     m_wrap;
		bool    m_bullet;

	};

	//-----------------------------------------------------------------------------
	// mvLabelText
	//-----------------------------------------------------------------------------
	class mvLabelText : public mvStringItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::LabelText)

			mvLabelText(const std::string& parent, const std::string& name, const std::string& value, mvColor color)
			: mvStringItemBase(parent, name, value), m_color(color)
		{
			m_label = FindRenderedTextEnd(m_name.c_str());
		}

		virtual void draw() override
		{

			

			if (m_color.specified)
				ImGui::PushStyleColor(ImGuiCol_Text, m_color);

			ImGui::Text("%s", m_value.c_str());

			if (m_color.specified)
				ImGui::PopStyleColor();

			ImGui::SameLine();
			ImGui::Text("%s", m_label.c_str());

		}

	private:

		mvColor m_color;

	};

}