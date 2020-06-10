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

				// Context Menu
				if (getPopup() != "")
					ImGui::OpenPopup(getPopup().c_str());
			}
			else
				m_value = false;
		}

	};

	class mvEndCollapsingHeader : public mvNoneItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::EndCollapsingHeader)

		mvEndCollapsingHeader(const std::string& parent)
			: mvNoneItemBase(parent, "EndCollapsingHeader")
		{}

		virtual void draw() override
		{
			mvApp::GetApp()->popParent();
		}

	};

}