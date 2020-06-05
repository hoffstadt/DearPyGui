#pragma once

#include "Core/AppItems/mvTypeBases.h"

namespace Marvel {

	class mvCollapsingHeader : public mvBoolItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::CollapsingHeader)

		mvCollapsingHeader(const std::string& parent, const std::string& name)
			: mvBoolItemBase(parent, name, false)
		{}

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

	};

}