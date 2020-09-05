#pragma once

#include "Core/AppItems/mvTypeBases.h"
#include "mvPythonTranslator.h"
#include <utility>
#include "mvAppItem.h"

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
//     * mvInputInt
//     * mvInputIntMulti
//     * mvInputFloat
//     * mvInputFloatMulti
//     * mvDragFloat
//     * mvDragInt
//     * mvSliderFloat
//     * mvSliderInt
//     * mvSliderFloatMulti
//     * mvSliderIntMulti
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// Helper typedefs
	//-----------------------------------------------------------------------------
	template<typename T>
	using ImGuiSlideCommand = bool(*)(const char*, T*, T, T, const char*, ImGuiSliderFlags);
	template<typename T>
	using ImGuiDragCommand = bool(*)(const char*, T*, float, T, T, const char*, ImGuiSliderFlags);
	using ImGuiIntInputCommand = bool(*)(const char*, int*, ImGuiInputTextFlags);
	using ImGuiFloatInputCommand = bool(*)(const char*, float*, const char*, ImGuiInputTextFlags);
	using ImGuiIntCommand = bool(*)(const char*, int*, ImGuiInputTextFlags);
	using ImGuiFloatCommand = bool(*)(const char*, float*, const char*, ImGuiInputTextFlags);

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

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());
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

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());
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

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());
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

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());
		}

		void updateData(const std::string& name) override
		{
			if (name == m_listDataSource)
			{
				PyObject* data = mvDataStorage::GetData(name);
				if (data == nullptr)
					return;

				m_names = ToStringVect(data);
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

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());
		}

		void updateData(const std::string& name) override
		{
			if (name == m_listDataSource)
			{
				PyObject* data = mvDataStorage::GetData(name);
				if (data == nullptr)
					return;

				m_names = ToStringVect(data);
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
			for (size_t i = 0; i < m_itemnames.size(); i++)
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

				// Regular Tooltip (simple)
				if (!getTip().empty() && ImGui::IsItemHovered())
					ImGui::SetTooltip("%s", getTip().c_str());
			}

		}

		void updateData(const std::string& name) override
		{
			if (name == m_listDataSource)
			{
				PyObject* data = mvDataStorage::GetData(name);
				if (data == nullptr)
					return;

				m_itemnames = ToStringVect(data);
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

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

		}

	private:

		std::string m_overlay;

	};

	//-----------------------------------------------------------------------------
	// mvInputInt
	//-----------------------------------------------------------------------------
	class mvInputInt : public mvIntItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::InputInt)

		mvInputInt(const std::string& parent, const std::string& name, int default_value, ImGuiInputTextFlags flags)
			: mvIntItemBase(parent, name, 1, default_value), m_flags(flags) {}

		void draw() override
		{

			if (ImGui::InputInt(m_label.c_str(), m_value.data(), 1, 100, m_flags))
			{
				if (!m_dataSource.empty())
					mvDataStorage::AddData(m_dataSource, getPyValue());

				mvApp::GetApp()->runCallback(m_callback, m_name);

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());
			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

		}

	private:

		ImGuiInputTextFlags m_flags = 0;

	};

	//-----------------------------------------------------------------------------
	// mvInputIntMulti
	//-----------------------------------------------------------------------------
	template<mvAppItemType AppItemType, int num, ImGuiIntInputCommand imguicommand>
	class mvInputIntMulti : public mvIntItemBase
	{

	public:

		MV_APPITEM_TYPE(AppItemType)

		mvInputIntMulti(const std::string& parent, const std::string& name, int default_value[2], ImGuiInputTextFlags flags)
			: mvIntItemBase(parent, name, num, default_value[0], default_value[1], default_value[2], default_value[3]),
			m_flags(flags){}

		void draw() override
		{

			if (imguicommand(m_label.c_str(), m_value.data(), m_flags))
			{
				if (!m_dataSource.empty())
					mvDataStorage::AddData(m_dataSource, getPyValue());

				mvApp::GetApp()->runCallback(m_callback, m_name);

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());
			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());

		}

	private:

		ImGuiInputTextFlags m_flags = 0;

	};

	//-----------------------------------------------------------------------------
	// mvInputFloat
	//-----------------------------------------------------------------------------
	class mvInputFloat : public mvFloatItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::InputFloat)

			mvInputFloat(const std::string& parent, const std::string& name, float default_value, std::string format, ImGuiInputTextFlags flags)
			: mvFloatItemBase(parent, name, 1, default_value), m_format(std::move(format)), m_flags(flags)
		{
		}

		void draw() override
		{
			if (ImGui::InputFloat(m_label.c_str(), m_value.data(), 0.0f, 0.0f, m_format.c_str(), m_flags))
			{
				if (!m_dataSource.empty())
					mvDataStorage::AddData(m_dataSource, getPyValue());

				mvApp::GetApp()->runCallback(m_callback, m_name);

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());
			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());
		}

	private:

		std::string         m_format;
		ImGuiInputTextFlags m_flags = 0;

	};

	//-----------------------------------------------------------------------------
	// mvInputFloatMulti
	//-----------------------------------------------------------------------------
	template<mvAppItemType AppItemType, int num, ImGuiFloatInputCommand imguicommand>
	class mvInputFloatMulti : public mvFloatItemBase
	{

	public:

		MV_APPITEM_TYPE(AppItemType)

			mvInputFloatMulti(const std::string& parent, const std::string& name, float* default_value, std::string  format, ImGuiInputTextFlags flags)
			: mvFloatItemBase(parent, name, num, default_value[0], default_value[1], default_value[2], default_value[3]), m_format(std::move(format)),
				m_flags(flags)
		{
		}

		void draw() override
		{
			if (imguicommand(m_label.c_str(), m_value.data(), m_format.c_str(), m_flags))
			{
				if (!m_dataSource.empty())
					mvDataStorage::AddData(m_dataSource, getPyValue());

				mvApp::GetApp()->runCallback(m_callback, m_name);

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());
			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());
		}

	private:

		std::string         m_format;
		ImGuiInputTextFlags m_flags = 0;

	};

	//-----------------------------------------------------------------------------
	// mvDragFloat
	//-----------------------------------------------------------------------------
	template<mvAppItemType AppItemType, int num, ImGuiDragCommand<float> imguicommand>
	class mvDragFloat : public mvFloatItemBase
	{

	public:

		MV_APPITEM_TYPE(AppItemType)

		mvDragFloat(const std::string& parent, const std::string& name, float* default_value, float speed,
				float minv, float maxv, std::string format, ImGuiInputTextFlags flags)
			: mvFloatItemBase(parent, name, num, default_value[0], default_value[1], default_value[2], default_value[3]),
			m_speed(speed), m_min(minv), m_max(maxv), m_format(std::move(format)), m_flags(flags)
		{
		}

		void draw() override
		{
			if (imguicommand(m_label.c_str(), m_value.data(), m_speed, m_min, m_max, m_format.c_str(), m_flags))
			{
				if (!m_dataSource.empty())
					mvDataStorage::AddData(m_dataSource, getPyValue());

				mvApp::GetApp()->runCallback(m_callback, m_name);

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());
			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());
		}

	private:

		float               m_speed;
		float               m_min;
		float               m_max;
		std::string         m_format;
		ImGuiInputTextFlags m_flags = 0;

	};

	//-----------------------------------------------------------------------------
	// mvDragInt
	//-----------------------------------------------------------------------------
	template<mvAppItemType AppItemType, int num, ImGuiDragCommand<int> imguicommand >
	class mvDragInt : public mvIntItemBase
	{

	public:

		MV_APPITEM_TYPE(AppItemType)

		mvDragInt(const std::string& parent, const std::string& name, int* default_value, float speed,
				int minv, float maxv, std::string format, ImGuiInputTextFlags flags)
			: mvIntItemBase(parent, name, num, default_value[0], default_value[1], default_value[2], default_value[3]),
			m_speed(speed), m_min(minv), m_max(maxv), m_format(std::move(format)), m_flags(flags)
		{
		}

		void draw() override
		{
			if (imguicommand(m_label.c_str(), m_value.data(), m_speed, m_min, m_max, m_format.c_str(), m_flags))
			{
				if (!m_dataSource.empty())
					mvDataStorage::AddData(m_dataSource, getPyValue());

				mvApp::GetApp()->runCallback(m_callback, m_name);

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());
			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());
		}

	private:

		float               m_speed;
		int                 m_min;
		int                 m_max;
		std::string         m_format;
		ImGuiInputTextFlags m_flags = 0;

	};

	//-----------------------------------------------------------------------------
	// mvSliderFloat
	//-----------------------------------------------------------------------------
	class mvSliderFloat : public mvFloatItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::SliderFloat)

			mvSliderFloat(const std::string& parent, const std::string& name, float default_value = 0.0f, float minv = 0.0f,
				float maxv = 1.0f, std::string  format = "%.3f", bool vertical = false, ImGuiInputTextFlags flags = 0)
			: mvFloatItemBase(parent, name, 1, default_value), m_min(minv), m_max(maxv), m_format(std::move(format)),
			m_vertical(vertical), m_flags(flags)
		{
		}

		void draw() override
		{
			if (m_vertical)
			{
				if ((float)m_height < 1.0f)
					m_height = 100.f;
				if ((float)m_width < 1.0f)
					m_width = 20.f;

				if (ImGui::VSliderFloat(m_label.c_str(), ImVec2((float)m_width, (float)m_height), m_value.data(), m_min, m_max, m_format.c_str()))
				{
					if (!m_dataSource.empty())
						mvDataStorage::AddData(m_dataSource, getPyValue());

					mvApp::GetApp()->runCallback(m_callback, m_name);

					// Context Menu
					if (!getPopup().empty())
						ImGui::OpenPopup(getPopup().c_str());
				}

				// Regular Tooltip (simple)
				if (!getTip().empty() && ImGui::IsItemHovered())
					ImGui::SetTooltip("%s", getTip().c_str());
			}
			else
			{
				if (ImGui::SliderFloat(m_label.c_str(), m_value.data(), m_min, m_max, m_format.c_str(), m_flags))
				{
					if (!m_dataSource.empty())
						mvDataStorage::AddData(m_dataSource, getPyValue());

					mvApp::GetApp()->runCallback(m_callback, m_name);

					// Context Menu
					if (!getPopup().empty())
						ImGui::OpenPopup(getPopup().c_str());
				}

				// Regular Tooltip (simple)
				if (!getTip().empty() && ImGui::IsItemHovered())
					ImGui::SetTooltip("%s", getTip().c_str());
			}
		}

	private:

		float               m_min;
		float               m_max;
		std::string         m_format;
		bool                m_vertical;
		ImGuiInputTextFlags m_flags;

	};

	//-----------------------------------------------------------------------------
	// mvSliderInt
	//-----------------------------------------------------------------------------
	class mvSliderInt : public mvIntItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::SliderInt)

			mvSliderInt(const std::string& parent, const std::string& name, int default_value = 0, int minv = 0,
				int maxv = 100, std::string  format = "%d", bool vertical = false, ImGuiInputTextFlags flags = 0)
			: mvIntItemBase(parent, name, 1, default_value), m_min(minv), m_max(maxv), m_format(std::move(format)), 
			m_vertical(vertical), m_flags(flags)
		{
		}

		void draw() override
		{
			if (m_vertical)
			{
				if ((float)m_height < 1.0f)
					m_height = 100.f;
				if ((float)m_width < 1.0f)
					m_width = 20.f;

				if (ImGui::VSliderInt(m_label.c_str(), ImVec2((float)m_width, (float)m_height), m_value.data(), m_min, m_max, m_format.c_str()))
				{
					if (!m_dataSource.empty())
						mvDataStorage::AddData(m_dataSource, getPyValue());

					mvApp::GetApp()->runCallback(m_callback, m_name);

					// Context Menu
					if (!getPopup().empty())
						ImGui::OpenPopup(getPopup().c_str());
				}

				// Regular Tooltip (simple)
				if (!getTip().empty() && ImGui::IsItemHovered())
					ImGui::SetTooltip("%s", getTip().c_str());
			}
			else
			{
				if (ImGui::SliderInt(m_label.c_str(), m_value.data(), m_min, m_max, m_format.c_str(), m_flags))
				{
					if (!m_dataSource.empty())
						mvDataStorage::AddData(m_dataSource, getPyValue());

					mvApp::GetApp()->runCallback(m_callback, m_name);

					// Context Menu
					if (!getPopup().empty())
						ImGui::OpenPopup(getPopup().c_str());
				}

				// Regular Tooltip (simple)
				if (!getTip().empty() && ImGui::IsItemHovered())
					ImGui::SetTooltip("%s", getTip().c_str());
			}
		}

	private:

		int                 m_min;
		int                 m_max;
		std::string         m_format;
		bool                m_vertical;
		ImGuiInputTextFlags m_flags;

	};

	//-----------------------------------------------------------------------------
	// mvSliderFloatMulti
	//-----------------------------------------------------------------------------
	template<mvAppItemType AppItemType, int num, ImGuiSlideCommand<float> imguicommand, typename T>
	class mvSliderFloatMulti : public mvFloatItemBase
	{

	public:

		MV_APPITEM_TYPE(AppItemType)

			mvSliderFloatMulti(const std::string& parent, const std::string& name, T* default_value, T minv, T maxv, std::string  format, ImGuiInputTextFlags flags)
			: mvFloatItemBase(parent, name, num, default_value[0], default_value[1], default_value[2], default_value[3]),
			m_min(minv), m_max(maxv), m_format(std::move(format)), m_flags(flags)
		{
		}

		void draw() override
		{
			if (imguicommand(m_label.c_str(), m_value.data(), m_min, m_max, m_format.c_str(), m_flags))
			{
				if (!m_dataSource.empty())
					mvDataStorage::AddData(m_dataSource, getPyValue());

				mvApp::GetApp()->runCallback(m_callback, m_name);

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());
			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());
		}

	private:

		T                   m_min;
		T                   m_max;
		std::string         m_format;
		ImGuiInputTextFlags m_flags = 0;

	};

	//-----------------------------------------------------------------------------
	// mvSliderIntMulti
	//-----------------------------------------------------------------------------
	template<mvAppItemType AppItemType, int num, ImGuiSlideCommand<int> imguicommand, typename T>
	class mvSliderIntMulti : public mvIntItemBase
	{

	public:

		MV_APPITEM_TYPE(AppItemType)

			mvSliderIntMulti(const std::string& parent, const std::string& name, T* default_value, T minv, T maxv, std::string  format, ImGuiInputTextFlags flags)
			: mvIntItemBase(parent, name, num, default_value[0], default_value[1], default_value[2], default_value[3]),
			m_min(minv), m_max(maxv), m_format(std::move(format)), m_flags(flags)
		{
		}

		void draw() override
		{
			if (imguicommand(m_label.c_str(), m_value.data(), m_min, m_max, m_format.c_str(), m_flags))
			{
				if (!m_dataSource.empty())
					mvDataStorage::AddData(m_dataSource, getPyValue());

				mvApp::GetApp()->runCallback(m_callback, m_name);

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());
			}

			// Regular Tooltip (simple)
			if (!getTip().empty() && ImGui::IsItemHovered())
				ImGui::SetTooltip("%s", getTip().c_str());
		}

	private:

		T                   m_min;
		T                   m_max;
		std::string         m_format;
		ImGuiInputTextFlags m_flags = 0;

	};
}