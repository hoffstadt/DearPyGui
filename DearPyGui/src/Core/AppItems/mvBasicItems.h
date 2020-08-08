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

		void draw() override
		{
			if(ImGui::Selectable(m_label.c_str(), &m_value))
			{

				if (!m_dataSource.empty())
					mvDataStorage::AddData(m_dataSource, getPyValue());

				mvApp::GetApp()->runCallback(m_callback, m_name);

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());
			}
		}

	};

	//-----------------------------------------------------------------------------
	// mvButton
	//-----------------------------------------------------------------------------
	class mvButton : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Button)

		mvButton(const std::string& parent, const std::string& name, bool small = false, 
			bool arrow = false, ImGuiDir direction = ImGuiDir_None)
			: mvAppItem(parent, name), m_small(small), m_arrow(arrow), m_direction(direction)
		{
		}

		void draw() override
		{
			if (m_small)
			{
				if (ImGui::SmallButton(m_label.c_str()))
				{

					mvApp::GetApp()->runCallback(m_callback, m_name);

					// Context Menu
					if (!getPopup().empty())
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
					if (!getPopup().empty())
						ImGui::OpenPopup(getPopup().c_str());
				}

				return;
			}

			if (ImGui::Button(m_label.c_str(), ImVec2((float)m_width, (float)m_height)))
			{

				mvApp::GetApp()->runCallback(m_callback, m_name);

				// Context Menu
				if (!getPopup().empty())
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

		void draw() override
		{
			if (ImGui::Checkbox(m_label.c_str(), &m_value))
			{
				if (!m_dataSource.empty())
					mvDataStorage::AddData(m_dataSource, getPyValue());

				mvApp::GetApp()->runCallback(m_callback, m_name);

				// Context Menu
				if (!getPopup().empty())
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

		mvCombo(const std::string& parent, const std::string& name, std::vector<std::string> itemnames, const std::string& default_value,
			std::string listDataSource = "")
			: mvStringItemBase(parent, name, default_value), m_names(std::move(itemnames)), m_listDataSource(std::move(listDataSource))
		{}

		void draw() override
		{

			if (ImGui::BeginCombo(m_label.c_str(), m_value.c_str())) // The second parameter is the label previewed before opening the combo.
			{
				for (const auto& name : m_names)
				{
					bool is_selected = (m_value == name);
					if (ImGui::Selectable((name + "##" + m_name).c_str(), is_selected))
					{
						if (!m_dataSource.empty())
							mvDataStorage::AddData(m_dataSource, getPyValue());

						m_value = name;
						mvApp::GetApp()->runCallback(m_callback, m_name);

					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
				}

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());

				ImGui::EndCombo();
			}
		}

		void updateData(const std::string& name) override
		{
			if (name == m_listDataSource)
			{
				PyObject* data = mvDataStorage::GetData(name);
				if (data == nullptr)
					return;

				m_names = mvPythonTranslator::ToStringVect(data);
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

		mvListbox(const std::string& parent, const std::string& name, std::vector<std::string> itemnames, int default_value = 0, int height = 3,
			std::string listDataSource = "")
			: mvIntItemBase(parent, name, 1, default_value), m_names(std::move(itemnames)), m_itemsHeight(height),
			m_listDataSource(std::move(listDataSource))
		{
			for (const std::string& item : m_names)
				m_charNames.emplace_back(item.c_str());
		}

		void draw() override
		{

			if (ImGui::ListBox(m_label.c_str(), &m_value[0], m_charNames.data(), m_names.size(), m_itemsHeight))
			{
				if (!m_dataSource.empty())
					mvDataStorage::AddData(m_dataSource, getPyValue());

				mvApp::GetApp()->runCallback(m_callback, m_name);

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());
			}
		}

		void updateData(const std::string& name) override
		{
			if (name == m_listDataSource)
			{
				PyObject* data = mvDataStorage::GetData(name);
				if (data == nullptr)
					return;

				m_names = mvPythonTranslator::ToStringVect(data);
				m_charNames.clear();
				for (const std::string& item : m_names)
					m_charNames.emplace_back(item.c_str());
			}
		}

	private:

		std::vector<std::string> m_names;
		int                      m_itemsHeight; // number of items to show (default -1)
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

		mvRadioButton(const std::string& parent, const std::string& name, std::vector<std::string> itemnames, int default_value,
			std::string secondaryDataSource = "")
			: mvIntItemBase(parent, name, 1, default_value), m_itemnames(std::move(itemnames)), m_listDataSource(std::move(secondaryDataSource))
		{
		}

		void draw() override
		{
			for (int i = 0; i < m_itemnames.size(); i++)
			{
				if (ImGui::RadioButton((m_itemnames[i] + "##" + m_name).c_str(), &m_value[0], i))
				{
					if (!m_dataSource.empty())
						mvDataStorage::AddData(m_dataSource, getPyValue());

					mvApp::GetApp()->runCallback(m_callback, m_name);

					// Context Menu
					if (!getPopup().empty())
						ImGui::OpenPopup(getPopup().c_str());
				}
			}

		}

		void updateData(const std::string& name) override
		{
			if (name == m_listDataSource)
			{
				PyObject* data = mvDataStorage::GetData(name);
				if (data == nullptr)
					return;

				m_itemnames = mvPythonTranslator::ToStringVect(data);
			}
		}

	private:

		std::vector<std::string> m_itemnames;
		std::string              m_listDataSource;

	};

	//-----------------------------------------------------------------------------
	// mvProgressBar
	//-----------------------------------------------------------------------------
	class mvProgressBar : public mvFloatItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::ProgressBar)

		mvProgressBar(const std::string& parent, const std::string& name, float default_value = 0.0f, std::string overlay = "")
			: mvFloatItemBase(parent, name, 1, default_value), m_overlay(std::move(overlay))
		{
		}

		void draw() override
		{

			ImGui::ProgressBar(m_value[0], ImVec2((float)m_width, (float)m_height), m_overlay.c_str());

			// Context Menu
			if (!getPopup().empty())
				ImGui::OpenPopup(getPopup().c_str());

		}

	private:

		std::string m_overlay;

	};

}