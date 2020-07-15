#pragma once

#include "Core/AppItems/mvTypeBases.h"
#include "mvPythonTranslator.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvSelectable
//     * mvButton
//     * mvCheckbox
//     * mvCombo
//     * mvListbox
//     * mvRadioButton
//     * mvProgressBar
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

				if (!m_dataSource.empty())
					mvApp::GetApp()->addData(m_dataSource, getPyValue());

				mvApp::GetApp()->runCallback(m_callback, m_name);

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

		mvButton(const std::string& parent, const std::string& name, bool small = false, 
			bool arrow = false, ImGuiDir direction = ImGuiDir_None)
			: mvNoneItemBase(parent, name), m_small(small), m_arrow(arrow), m_direction(direction)
		{
		}

		virtual void draw() override
		{
			if (m_small)
			{
				if (ImGui::SmallButton(m_label.c_str()))
				{

					mvApp::GetApp()->runCallback(m_callback, m_name);

					// Context Menu
					if (getPopup() != "")
						ImGui::OpenPopup(getPopup().c_str());
				}

				return;
			}

			if (m_arrow)
			{
				if (ImGui::ArrowButton(m_label.c_str(), m_direction))
				{

					mvApp::GetApp()->runCallback(m_callback, m_name);

					// Context Menu
					if (getPopup() != "")
						ImGui::OpenPopup(getPopup().c_str());
				}

				return;
			}

			if (ImGui::Button(m_label.c_str(), ImVec2(m_width, m_height)))
			{

				mvApp::GetApp()->runCallback(m_callback, m_name);

				// Context Menu
				if (getPopup() != "")
					ImGui::OpenPopup(getPopup().c_str());
			}
		}

	private:

		bool     m_small;
		bool     m_arrow;
		ImGuiDir m_direction = ImGuiDir_None;

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
				if (!m_dataSource.empty())
					mvApp::GetApp()->addData(m_dataSource, getPyValue());

				mvApp::GetApp()->runCallback(m_callback, m_name);

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

		mvCombo(const std::string& parent, const std::string& name, const std::vector<std::string>& itemnames, const std::string& default_value,
			const std::string& listDataSource = "")
			: mvStringItemBase(parent, name, default_value), m_names(itemnames), m_listDataSource(listDataSource)
		{}

		virtual void draw() override
		{

			if (ImGui::BeginCombo(m_label.c_str(), m_value.c_str())) // The second parameter is the label previewed before opening the combo.
			{
				for (int i = 0; i < m_names.size(); i++)
				{
					bool is_selected = (m_value == m_names[i]);
					if (ImGui::Selectable((m_names[i] + "##" + m_name).c_str(), is_selected))
					{
						if (!m_dataSource.empty())
							mvApp::GetApp()->addData(m_dataSource, getPyValue());

						m_value = m_names[i];
						mvApp::GetApp()->runCallback(m_callback, m_name);

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

		virtual void updateData(const std::string& name) override
		{
			if (name == m_listDataSource)
			{
				PyObject* data = mvApp::GetApp()->getData(name);
				if (data == nullptr)
					return;

				m_names = mvPythonTranslator::getStringVec(data);
			}
		}

	private:

		std::vector<std::string> m_names;
		std::string              m_listDataSource;

	};

	//-----------------------------------------------------------------------------
	// mvListbox
	//-----------------------------------------------------------------------------
	class mvListbox : public mvIntItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Listbox)

		mvListbox(const std::string& parent, const std::string& name, const std::vector<std::string>& itemnames, int default_value = 0, int height = 3,
			const std::string& listDataSource = "")
			: mvIntItemBase(parent, name, 1, default_value), m_names(itemnames), m_height(height), m_listDataSource(listDataSource)
		{
			for (const std::string& name : m_names)
				m_charNames.emplace_back(name.c_str());
		}

		virtual void draw() override
		{

			if (ImGui::ListBox(m_label.c_str(), &m_value[0], m_charNames.data(), m_names.size(), m_height))
			{
				if (!m_dataSource.empty())
					mvApp::GetApp()->addData(m_dataSource, getPyValue());

				mvApp::GetApp()->runCallback(m_callback, m_name);

				// Context Menu
				if (getPopup() != "")
					ImGui::OpenPopup(getPopup().c_str());
			}
		}

		virtual void updateData(const std::string& name) override
		{
			if (name == m_listDataSource)
			{
				PyObject* data = mvApp::GetApp()->getData(name);
				if (data == nullptr)
					return;

				m_names = mvPythonTranslator::getStringVec(data);
				m_charNames.clear();
				for (const std::string& name : m_names)
					m_charNames.emplace_back(name.c_str());
			}
		}

	private:

		std::vector<std::string> m_names;
		int                      m_height; // number of items to show (default -1)
		std::vector<const char*> m_charNames;
		std::string              m_listDataSource;

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
				if (ImGui::RadioButton((m_itemnames[i] + "##" + m_name).c_str(), &m_value[0], i))
				{
					if (!m_dataSource.empty())
						mvApp::GetApp()->addData(m_dataSource, getPyValue());

					mvApp::GetApp()->runCallback(m_callback, m_name);

					// Context Menu
					if (getPopup() != "")
						ImGui::OpenPopup(getPopup().c_str());
				}
			}

		}

	private:

		std::vector<std::string> m_itemnames;

	};

	//-----------------------------------------------------------------------------
	// mvProgressBar
	//-----------------------------------------------------------------------------
	class mvProgressBar : public mvFloatItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::ProgressBar)

		mvProgressBar(const std::string& parent, const std::string& name, float default_value = 0.0f, const std::string& overlay = "")
			: mvFloatItemBase(parent, name, 1, default_value), m_overlay(overlay)
		{
		}

		virtual void draw() override
		{

			ImGui::ProgressBar(m_value[0], ImVec2(m_width, m_height), m_overlay.c_str());

			// Context Menu
			if (getPopup() != "")
				ImGui::OpenPopup(getPopup().c_str());

		}

	private:

		std::string m_overlay;

	};

}