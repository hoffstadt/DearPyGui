#pragma once

#include "Core/mvAppItem.h"

namespace Marvel {

	class mvCheckbox : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Checkbox)

		mvCheckbox(const std::string& parent, const std::string& name, bool default_value)
			: mvAppItem(parent, name), m_value(default_value)
		{
		}

		virtual PyObject* getPyValue() override
		{
			PyObject* pvalue = Py_BuildValue("i", m_value);
			return pvalue;
		}

		virtual void draw() override
		{
			if (ImGui::Checkbox(m_label.c_str(), &m_value))
				mvApp::GetApp()->triggerCallback(m_callback, m_name);
		}

	private:

		bool m_value;

	};

}