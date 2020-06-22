#pragma once

#include "Core/AppItems/mvTypeBases.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvColorEdit4
//
//-----------------------------------------------------------------------------

namespace Marvel {

	class mvColorEdit4 : public mvFloatItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::ColorEdit4)

		mvColorEdit4(const std::string& parent, const std::string& name, float r, float g, float b, float a)
			: mvFloatItemBase(parent, name, 4, r, g, b, a)
		{}

		virtual void draw() override
		{

			if (ImGui::ColorEdit4(m_label.c_str(), m_value))
			{
				auto threadpool = mvThreadPool::GetThreadPool();
				threadpool->submit(std::bind(&mvApp::triggerCallback, mvApp::GetApp(), m_callback, m_name));

				// Context Menu
				if (getPopup() != "")
					ImGui::OpenPopup(getPopup().c_str());
			}
		}

	};

}