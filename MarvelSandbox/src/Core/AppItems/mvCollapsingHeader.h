#pragma once

#include "Core/mvAppItem.h"

namespace Marvel {

	class mvCollapsingHeader : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::CollapsingHeader)

		mvCollapsingHeader(const std::string& parent, const std::string& name)
			: mvAppItem(parent, name)
		{
			m_value = false;
		}

		virtual PyObject* getPyValue() override
		{
			PyObject* pvalue = Py_BuildValue("i", m_value);
			return pvalue;
		}

		virtual void draw() override
		{
			// create menu and see if its selected
			if (ImGui::CollapsingHeader(m_label.c_str(), 0))
			{
				// set current menu value true
				m_value = true;
				showAll();
			}
			else
				m_value = false;
		}

		inline bool getValue() const { return m_value; }

	private:

		bool m_value;

	};

}