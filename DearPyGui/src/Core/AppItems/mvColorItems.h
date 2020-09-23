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

	using ImGuiColorCommand = bool(*)(const char*, float*, ImGuiColorEditFlags);

	//-----------------------------------------------------------------------------
	// mvColorItem
	//-----------------------------------------------------------------------------
	template<mvAppItemType AppItemType, ImGuiColorCommand imguicommand>
	class mvColorItem : public mvColorItemBase
	{

	public:

		MV_APPITEM_TYPE(AppItemType, "add_color_edit3")

			mvColorItem(const std::string& name, mvColor color)
			: mvColorItemBase(name, color)
		{}

		void draw() override
		{

			if (imguicommand(m_label.c_str(), m_value.data(), 0))
			{
				if (!m_dataSource.empty())
					mvDataStorage::AddData(m_dataSource, getPyValue());

				mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());
			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());
		}

	};

	//-----------------------------------------------------------------------------
	// mvColorPicker4
	//-----------------------------------------------------------------------------
	class mvColorPicker4 : public mvColorItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::ColorPicker4, "add_color_picker3")

		mvColorPicker4(const std::string& name, mvColor color)
			: mvColorItemBase(name, color)
		{}

		void draw() override
		{

			if (ImGui::ColorPicker4(m_label.c_str(), m_value.data()))
			{
				if (!m_dataSource.empty())
					mvDataStorage::AddData(m_dataSource, getPyValue());

				mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());
			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());
		}

	};

}