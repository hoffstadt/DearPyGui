#pragma once

#include "Core/AppItems/mvTypeBases.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvSelectable
//     * mvButton
//     * mvCheckbox
//     * mvCombo
//     * mvListbox
//     * mvRadioButton
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvSelectable
	//-----------------------------------------------------------------------------
	class mvSelectable : public mvBoolItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Selectable)

		mvSelectable(const std::string& parent, const std::string& name, bool default_value)
			: mvBoolItemBase(parent, name, default_value)
		{
		}

		virtual void draw() override
		{
			if(ImGui::Selectable(m_label.c_str(), &m_value))
			{
				mvApp::GetApp()->triggerCallback(m_callback, m_name);

				// Context Menu
				if (getPopup() != "")
					ImGui::OpenPopup(getPopup().c_str());
			}
		}

	};

	//-----------------------------------------------------------------------------
	// mvButton
	//-----------------------------------------------------------------------------
	class mvButton : public mvNoneItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Button)

		mvButton(const std::string& parent, const std::string& name)
			: mvNoneItemBase(parent, name)
		{}

		virtual void draw() override
		{
			if (ImGui::Button(m_label.c_str()))
			{
				mvApp::GetApp()->triggerCallback(m_callback, m_name);

				// Context Menu
				if (getPopup() != "")
					ImGui::OpenPopup(getPopup().c_str());
			}
		}

	};

	//-----------------------------------------------------------------------------
	// mvCheckbox
	//-----------------------------------------------------------------------------
	class mvCheckbox : public mvBoolItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Checkbox)

		mvCheckbox(const std::string& parent, const std::string& name, bool default_value)
			: mvBoolItemBase(parent, name, default_value)
		{
		}

		virtual void draw() override
		{
			if (ImGui::Checkbox(m_label.c_str(), &m_value))
			{
				mvApp::GetApp()->triggerCallback(m_callback, m_name);

				// Context Menu
				if (getPopup() != "")
					ImGui::OpenPopup(getPopup().c_str());
			}
		}

	};

	//-----------------------------------------------------------------------------
	// mvCombo
	//-----------------------------------------------------------------------------
	class mvCombo : public mvStringItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Combo)

		mvCombo(const std::string& parent, const std::string& name, const std::vector<std::string>& itemnames, const std::string& default_value)
			: mvStringItemBase(parent, name, default_value), m_names(itemnames)
		{}

		virtual void draw() override
		{

			if (ImGui::BeginCombo(m_label.c_str(), m_value.c_str())) // The second parameter is the label previewed before opening the combo.
			{
				for (int i = 0; i < m_names.size(); i++)
				{
					bool is_selected = (m_value == m_names[i]);
					if (ImGui::Selectable(m_names[i].c_str(), is_selected))
					{
						m_value = m_names[i];
						mvApp::GetApp()->triggerCallback(m_callback, m_name);
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
				}

				// Context Menu
				if (getPopup() != "")
					ImGui::OpenPopup(getPopup().c_str());

				ImGui::EndCombo();
			}
		}

	private:

		std::vector<std::string> m_names;

	};

	//-----------------------------------------------------------------------------
	// mvListbox
	//-----------------------------------------------------------------------------
	class mvListbox : public mvIntItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Listbox)

		mvListbox(const std::string& parent, const std::string& name, const std::vector<std::string>& itemnames, int default_value = 0, int height = -1)
			: mvIntItemBase(parent, name, 1, default_value), m_names(itemnames), m_height(height)
		{
		}

		virtual void draw() override
		{
			std::vector<const char*> names;
			for (const std::string& name : m_names)
				names.emplace_back(name.c_str());

			if (ImGui::ListBox(m_label.c_str(), &m_value[0], names.data(), m_names.size(), m_height))
			{
				mvApp::GetApp()->triggerCallback(m_callback, m_name);

				// Context Menu
				if (getPopup() != "")
					ImGui::OpenPopup(getPopup().c_str());
			}
		}

	private:

		std::vector<std::string> m_names;
		int                      m_height; // number of items to show (default -1)

	};

	//-----------------------------------------------------------------------------
	// mvRadioButton
	//-----------------------------------------------------------------------------
	class mvRadioButton : public mvIntItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::RadioButtons)

		mvRadioButton(const std::string& parent, const std::string& name, const std::vector<std::string>& itemnames, int default_value)
			: mvIntItemBase(parent, name, 1, default_value)
		{
			m_itemnames = itemnames;
		}

		virtual void draw() override
		{
			for (int i = 0; i < m_itemnames.size(); i++)
			{
				if (ImGui::RadioButton(m_itemnames[i].c_str(), &m_value[0], i))
				{
					mvApp::GetApp()->triggerCallback(m_callback, m_name);

					// Context Menu
					if (getPopup() != "")
						ImGui::OpenPopup(getPopup().c_str());
				}
			}

			//if (m_tip != "" && ImGui::IsItemHovered())
			//	ImGui::SetTooltip(m_tip.c_str());
		}

	private:

		std::vector<std::string> m_itemnames;

	};

}