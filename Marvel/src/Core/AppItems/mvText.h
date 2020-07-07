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

			ImGui::Text(m_value.c_str());

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
		}

		virtual void draw() override
		{

			if (m_color.specified)
				ImGui::PushStyleColor(ImGuiCol_Text, m_color);

			ImGui::Text(m_value.c_str());

			if (m_color.specified)
				ImGui::PopStyleColor();

			ImGui::SameLine();
			ImGui::Text(m_label.c_str());

		}

	private:

		mvColor m_color;

	};

}