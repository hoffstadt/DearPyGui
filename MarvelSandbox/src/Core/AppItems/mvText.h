#pragma once

#include "Core/AppItems/mvTypeBases.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvLabelText
	//-----------------------------------------------------------------------------
	class mvText : public mvNoneItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Text)

		mvText(const std::string& parent, const std::string& name, int wrap, mvColor color)
			: mvNoneItemBase(parent, name), m_wrap(wrap), m_color(color)
		{
		}

		virtual void draw() override
		{

			if (m_color.specified)
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(m_color.r, m_color.g, m_color.b, m_color.a));

			if (m_wrap != 0)
				ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + m_wrap);

			ImGui::Text(m_label.c_str());

			if (m_wrap != 0)
				ImGui::PopTextWrapPos();

			if (m_color.specified)
				ImGui::PopStyleColor();
		}

	private:

		mvColor m_color;
		int     m_wrap;

	};

	//-----------------------------------------------------------------------------
	// mvLabelText
	//-----------------------------------------------------------------------------
	class mvLabelText : public mvStringItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::LabelText)

			mvLabelText(const std::string& parent, const std::string& name, const std::string& value, int wrap, mvColor color)
			: mvStringItemBase(parent, name, value), m_wrap(wrap), m_color(color)
		{
		}

		virtual void draw() override
		{

			if (m_wrap != 0)
				ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + m_wrap);

			if (m_color.specified)
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(m_color.r, m_color.g, m_color.b, m_color.a));

			ImGui::Text(m_value.c_str());

			if (m_color.specified)
				ImGui::PopStyleColor();

			ImGui::SameLine();
			ImGui::Text(m_label.c_str());

			if (m_wrap != 0)
				ImGui::PopTextWrapPos();

		}

	private:

		mvColor      m_color;
		int          m_wrap;

	};

}