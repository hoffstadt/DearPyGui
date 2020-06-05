#pragma once

#include "Core/AppItems/mvTypeBases.h"

namespace Marvel {

	class mvInputFloat : public mvFloatItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::InputFloat)

		mvInputFloat(const std::string& parent, const std::string& name, float default_value)
			: mvFloatItemBase(parent, name, 1, default_value)
		{
		}

		virtual void draw() override
		{
			if (ImGui::InputFloat(m_label.c_str(), m_value))
				mvApp::GetApp()->triggerCallback(m_callback, m_name);
		}

	};

}