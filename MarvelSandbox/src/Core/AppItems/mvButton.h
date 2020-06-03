#pragma once

#include "Core/mvAppItem.h"

namespace Marvel {

	class mvButton : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Button)
		MV_NORETURN_VALUE()

		mvButton(const std::string& parent, const std::string& name)
			: mvAppItem(parent, name)
		{}

		virtual void draw() override
		{
			if (ImGui::Button(m_label.c_str()))
				mvApp::GetApp()->triggerCallback(m_callback, m_name);
		}

	};

}