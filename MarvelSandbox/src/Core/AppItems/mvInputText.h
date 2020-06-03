#pragma once

#include "Core/mvAppItem.h"

namespace Marvel {

	class mvInputText : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::InputText)

		mvInputText(const std::string& parent, const std::string& name, const std::string& hint = "")
			: mvAppItem(parent, name), m_hint(hint)
		{
			m_value = new std::string();
		}

		virtual PyObject* getPyValue() override
		{
			PyObject* pvalue = Py_BuildValue("s", m_value->c_str());

			return pvalue;
		}

		virtual void draw() override
		{
			if (m_hint == "")
			{
				if (ImGui::InputText(m_label.c_str(), (char*)m_value->c_str(), m_value->capacity() + 1))
					mvApp::GetApp()->triggerCallback(m_callback, m_name);
			}

			else
			{
				if (ImGui::InputTextWithHint(m_label.c_str(), m_hint.c_str(), (char*)m_value->c_str(), m_value->capacity() + 1))
					mvApp::GetApp()->triggerCallback(m_callback, m_name);
			}
		}

		inline const std::string& getValue() const { return *m_value; }

	private:

		std::string m_hint;
		std::string* m_value;

	};

}