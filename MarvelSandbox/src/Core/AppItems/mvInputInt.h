#pragma once

#include "Core/mvAppItem.h"

namespace Marvel {

	class mvInputInt : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::InputInt)

		mvInputInt(const std::string& parent, const std::string& name, int default_value)
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

			if (ImGui::InputInt(m_label.c_str(), &m_value))
				mvApp::GetApp()->triggerCallback(m_callback, m_name);

		}

	private:

		int m_value;

	};

}