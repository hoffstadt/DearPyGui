#pragma once

#include "Core/mvAppItem.h"

namespace Marvel {

	class mvText : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Text)
		MV_NORETURN_VALUE()

		mvText(const std::string& parent, const std::string& name, int wrap, mvColor color)
			: mvAppItem(parent, name), m_wrap(wrap), m_color(color)
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

}