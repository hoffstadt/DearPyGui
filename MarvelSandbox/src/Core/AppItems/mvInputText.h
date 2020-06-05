#pragma once

#include "Core/AppItems/mvTypeBases.h"

namespace Marvel {

	class mvInputText : public mvStringItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::InputText)

		mvInputText(const std::string& parent, const std::string& name, const std::string& hint = "")
			: mvStringItemBase(parent, name, ""), m_hint(hint)
		{
		}

		virtual void draw() override
		{
			if (m_hint == "")
			{
				if (ImGui::InputText(m_label.c_str(), (char*)m_value.c_str(), m_value.capacity() + 1))
					mvApp::GetApp()->triggerCallback(m_callback, m_name);
			}

			else
			{
				if (ImGui::InputTextWithHint(m_label.c_str(), m_hint.c_str(), (char*)m_value.c_str(), m_value.capacity() + 1))
					mvApp::GetApp()->triggerCallback(m_callback, m_name);
			}
		}

	private:

		std::string m_hint;

	};

}