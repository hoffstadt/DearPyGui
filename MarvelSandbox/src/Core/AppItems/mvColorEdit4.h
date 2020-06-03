#pragma once

#include "Core/mvAppItem.h"

namespace Marvel {

	class mvColorEdit4 : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::ColorEdit4)

		mvColorEdit4(const std::string& parent, const std::string& name, float r, float g, float b, float a)
			: mvAppItem(parent, name)
		{
			m_value[0] = r;
			m_value[1] = g;
			m_value[2] = b;
			m_value[3] = a;
		}

		virtual PyObject* getPyValue() override
		{
			PyObject* pvalue = Py_BuildValue("(ffff)", m_value[0], m_value[1], m_value[2], m_value[3]);
			return pvalue;
		}

		virtual void draw() override
		{

			if (ImGui::ColorEdit4(m_label.c_str(), m_value))
				mvApp::GetApp()->triggerCallback(m_callback, m_name);
		}

	private:

		float m_value[4];

	};

}