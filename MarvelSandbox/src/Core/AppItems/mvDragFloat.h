#pragma once

#include "Core/AppItems/mvTypeBases.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvDragFloat
//
//-----------------------------------------------------------------------------

namespace Marvel {
	
	class mvDragFloat : public mvFloatItemBase
	{
	public:
		MV_APPITEM_TYPE(mvAppItemType::DragFloat)

		mvDragFloat(const std::string& parent, const std::string& name, float default_value)
			: mvFloatItemBase(parent, name, 1, default_value)
		{
		}

		virtual void draw() override
		{
			if (ImGui::DragFloat(m_label.c_str(), m_value))
			{
				mvApp::GetApp()->triggerCallback(m_callback, m_name);

				// Context Menu
				if (getPopup() != "")
					ImGui::OpenPopup(getPopup().c_str());
			}
		}
	};

}