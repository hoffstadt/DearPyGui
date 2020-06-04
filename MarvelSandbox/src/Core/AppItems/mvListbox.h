#pragma once

#include "Core/mvAppItem.h"

namespace Marvel {

	class mvListbox : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Listbox)

			mvListbox(const std::string& parent, const std::string& name, const std::vector<std::string>& itemnames, int default_value = 0, int height = -1)
			: mvAppItem(parent, name), m_names(itemnames), m_value(default_value), m_height(height)
		{
		}

		virtual PyObject* getPyValue() override
		{
			PyObject* pvalue = Py_BuildValue("s", m_names[m_value].c_str());
			return pvalue;
		}

		virtual void draw() override
		{
			std::vector<const char*> names;
			for (const std::string& name : m_names)
				names.emplace_back(name.c_str());

			if (ImGui::ListBox(m_label.c_str(), &m_value, names.data(), m_names.size(), m_height))
				mvApp::GetApp()->triggerCallback(m_callback, m_name);
		}

	private:

		std::vector<std::string> m_names;
		int                      m_value;
		int                      m_height; // number of items to show (default -1)

	};

}