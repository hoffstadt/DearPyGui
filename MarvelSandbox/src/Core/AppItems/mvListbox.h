#pragma once

#include "Core/AppItems/mvTypeBases.h"

namespace Marvel {

	class mvListbox : public mvIntItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Listbox)

		mvListbox(const std::string& parent, const std::string& name, const std::vector<std::string>& itemnames, int default_value = 0, int height = -1)
			: mvIntItemBase(parent, name, 1, default_value), m_names(itemnames), m_height(height)
		{
		}

		virtual void draw() override
		{
			std::vector<const char*> names;
			for (const std::string& name : m_names)
				names.emplace_back(name.c_str());

			if (ImGui::ListBox(m_label.c_str(), &m_value[0], names.data(), m_names.size(), m_height))
				mvApp::GetApp()->triggerCallback(m_callback, m_name);
		}

	private:

		std::vector<std::string> m_names;
		int                      m_height; // number of items to show (default -1)

	};

}