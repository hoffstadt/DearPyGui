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
	// mvColorButton
	//-----------------------------------------------------------------------------
	class mvColorButton : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::ColorButton, "add_color_button")

		mvColorButton(const std::string& name, const mvColor& color)
			: mvAppItem(name), m_color(color.toVec4())
		{
		}

		void draw() override
		{

			if (ImGui::ColorButton(m_label.c_str(), m_color, ImGuiColorEditFlags_AlphaPreview, ImVec2((float)m_width, (float)m_height)))
			{

				mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());
			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());
		}

		void setExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;

			if (PyObject* item = PyDict_GetItemString(dict, "color")) m_color = ToColor(item).toVec4();
		}

		void getExtraConfigDict(PyObject* dict) override
		{
			if (dict == nullptr)
				return;
			mvGlobalIntepreterLock gil;
			PyDict_SetItemString(dict, "color", ToPyColor({(int)m_color.x*255, (int)m_color.y * 255, (int)m_color.z * 255, (int)m_color.w * 255 }));
		}

	private:

		ImVec4 m_color;

	};

	//-----------------------------------------------------------------------------
	// mvColorEdit3
	//-----------------------------------------------------------------------------
	class mvColorEdit3 : public mvFloat3PtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::ColorEdit3, "add_color_edit3")

		mvColorEdit3(const std::string& name, float* color, const std::string& dataSource)
			: mvFloat3PtrBase(name, color, dataSource)
		{}

		void draw() override
		{

			if (ImGui::ColorEdit3(m_label.c_str(), m_value, 0))
			{

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
	// mvColorEdit4
	//-----------------------------------------------------------------------------
	class mvColorEdit4 : public mvFloat4PtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::ColorEdit4, "add_color_edit4")

		mvColorEdit4(const std::string& name, float* color, const std::string& dataSource)
			: mvFloat4PtrBase(name, color, dataSource)
		{}

		void draw() override
		{

			if (ImGui::ColorEdit4(m_label.c_str(), m_value, 0))
			{

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
	// mvColorPicker3
	//-----------------------------------------------------------------------------
	class mvColorPicker3 : public mvFloat3PtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::ColorPicker3, "add_color_picker3")

		mvColorPicker3(const std::string& name, float* color, const std::string& dataSource)
			: mvFloat3PtrBase(name, color, dataSource)
		{}

		void draw() override
		{

			if (ImGui::ColorPicker3(m_label.c_str(), m_value, 0))
			{

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
	class mvColorPicker4 : public mvFloat4PtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::ColorPicker4, "add_color_picker4")

		mvColorPicker4(const std::string& name, float* color, const std::string& dataSource)
			: mvFloat4PtrBase(name, color, dataSource)
		{}

		void draw() override
		{

			if (ImGui::ColorPicker4(m_label.c_str(), m_value))
			{

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