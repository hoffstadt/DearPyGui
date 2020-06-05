#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	class mvCheckbox : public mvBoolItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Checkbox)

		mvCheckbox(const std::string& parent, const std::string& name, bool default_value)
			: mvBoolItemBase(parent, name, default_value)
		{
		}

		virtual void draw() override
		{
			if (ImGui::Checkbox(m_label.c_str(), &m_value))
				mvApp::GetApp()->triggerCallback(m_callback, m_name);
		}

	};

}