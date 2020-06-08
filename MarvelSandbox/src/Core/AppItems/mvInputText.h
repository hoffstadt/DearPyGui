#pragma once

#include "Core/AppItems/mvTypeBases.h"

namespace Marvel {

	class mvInputText : public mvStringItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::InputText)

		mvInputText(const std::string& parent, const std::string& name, const std::string& hint, bool multiline)
			: mvStringItemBase(parent, name, ""), m_hint(hint), m_multiline(multiline)
		{
		}

		virtual void draw() override
		{
			if (m_multiline)
				m_hint = "";

			if (m_hint == "")
			{
				if(m_multiline)
				{
					if (ImGui::InputTextMultiline(m_label.c_str(), (char*)m_value.c_str(), m_value.capacity() + 1))
						mvApp::GetApp()->triggerCallback(m_callback, m_name);
				}
				else
				{
					if (ImGui::InputText(m_label.c_str(), (char*)m_value.c_str(), m_value.capacity() + 1))
						mvApp::GetApp()->triggerCallback(m_callback, m_name);
				}
			}

			else
			{
				if (ImGui::InputTextWithHint(m_label.c_str(), m_hint.c_str(), (char*)m_value.c_str(), m_value.capacity() + 1))
					mvApp::GetApp()->triggerCallback(m_callback, m_name);
			}
		}

	private:

		std::string m_hint;
		bool        m_multiline;

	};

}