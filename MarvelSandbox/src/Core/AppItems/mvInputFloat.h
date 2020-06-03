#pragma once

#include "Core/mvAppItem.h"

namespace Marvel {

	class mvInputFloat : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::InputFloat)

		mvInputFloat(const std::string& parent, const std::string& name, float default_value)
			: mvAppItem(parent, name), m_value(default_value)
		{
		}

		virtual PyObject* getPyValue() override
		{
			PyObject* pvalue = Py_BuildValue("f", m_value);
			return pvalue;
		}

		virtual void draw() override
		{

			if (ImGui::InputFloat(m_label.c_str(), &m_value))
				mvApp::GetApp()->triggerCallback(m_callback, m_name);

		}

	private:

		float m_value;

	};

}