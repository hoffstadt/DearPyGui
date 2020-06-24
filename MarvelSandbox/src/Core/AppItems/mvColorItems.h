#pragma once

#include "Core/AppItems/mvTypeBases.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvColorEdit3
//     * mvColorEdit4
//     * mvColorPicker3
//     * mvColorPicker4
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvColorEdit3
	//-----------------------------------------------------------------------------
	class mvColorEdit3 : public mvColorItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::ColorEdit3)

		mvColorEdit3(const std::string& parent, const std::string& name, mvColor color)
			: mvColorItemBase(parent, name, color)
		{}

		virtual void draw() override
		{

			if (ImGui::ColorEdit3(m_label.c_str(), m_value))
			{
				mvApp::GetApp()->runCallback(m_callback, m_name);

				// Context Menu
				if (getPopup() != "")
					ImGui::OpenPopup(getPopup().c_str());
			}
		}

	};

	//-----------------------------------------------------------------------------
	// mvColorEdit4
	//-----------------------------------------------------------------------------
	class mvColorEdit4 : public mvColorItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::ColorEdit4)

		mvColorEdit4(const std::string& parent, const std::string& name, mvColor color)
			: mvColorItemBase(parent, name, color)
		{}

		virtual void draw() override
		{

			if (ImGui::ColorEdit4(m_label.c_str(), m_value))
			{
				mvApp::GetApp()->runCallback(m_callback, m_name);

				// Context Menu
				if (getPopup() != "")
					ImGui::OpenPopup(getPopup().c_str());
			}
		}

	};

	//-----------------------------------------------------------------------------
	// mvColorPicker3
	//-----------------------------------------------------------------------------
	class mvColorPicker3 : public mvColorItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::ColorPicker3)

		mvColorPicker3(const std::string& parent, const std::string& name, mvColor color)
			: mvColorItemBase(parent, name, color)
		{}

		virtual void draw() override
		{

			if (ImGui::ColorPicker3(m_label.c_str(), m_value))
			{
				mvApp::GetApp()->runCallback(m_callback, m_name);

				// Context Menu
				if (getPopup() != "")
					ImGui::OpenPopup(getPopup().c_str());
			}
		}

	};

	//-----------------------------------------------------------------------------
	// mvColorPicker4
	//-----------------------------------------------------------------------------
	class mvColorPicker4 : public mvColorItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::ColorPicker4)

		mvColorPicker4(const std::string& parent, const std::string& name, mvColor color)
			: mvColorItemBase(parent, name, color)
		{}

		virtual void draw() override
		{

			if (ImGui::ColorPicker4(m_label.c_str(), m_value))
			{
				mvApp::GetApp()->runCallback(m_callback, m_name);

				// Context Menu
				if (getPopup() != "")
					ImGui::OpenPopup(getPopup().c_str());
			}
		}

	};

}