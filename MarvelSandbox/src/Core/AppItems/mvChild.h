#pragma once

#include "Core/AppItems/mvTypeBases.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvChild
	//-----------------------------------------------------------------------------
	class mvChild : public mvBoolItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Child)

		mvChild(const std::string& parent, const std::string& name, int width, int height)
			: mvBoolItemBase(parent, name, false)
		{
			m_width = width;
			m_height = height;
		}

		virtual void draw() override
		{
			ImGui::BeginChild(m_label.c_str(), ImVec2(float(m_width), float(m_height)), true);

			mvApp::GetApp()->pushParent(this);

			// set current child value true
			m_value = true;

			if (m_tip != "" && ImGui::IsItemHovered())
				ImGui::SetTooltip(m_tip.c_str());
		}

	private:

		int m_height;

	};

	//-----------------------------------------------------------------------------
	// mvEndChild
	//-----------------------------------------------------------------------------
	class mvEndChild : public mvNoneItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::EndChild)

		mvEndChild(const std::string& parent)
			: mvNoneItemBase(parent, "Endgroup")
		{
		}

		virtual void draw() override
		{
			mvApp::GetApp()->popParent();
			ImGui::EndChild();
		}

	};

}