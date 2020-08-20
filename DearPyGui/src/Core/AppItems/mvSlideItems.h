#pragma once

#include <utility>

#include "Core/AppItems/mvTypeBases.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvSliderFloat
//     * mvSliderInt
//     * mvSliderFloatMulti
//     * mvSliderIntMulti
//
//-----------------------------------------------------------------------------

namespace Marvel {

	template<typename T>
	using ImGuiSlideCommand = bool(*)(const char*, T*, T, T, const char*, ImGuiSliderFlags);

	//-----------------------------------------------------------------------------
	// mvSliderFloat
	//-----------------------------------------------------------------------------
	class mvSliderFloat : public mvFloatItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::SliderFloat)

		mvSliderFloat(const std::string& parent, const std::string& name, float default_value=0.0f, float minv=0.0f, 
			float maxv=1.0f, std::string  format="%.3f", bool vertical = false)
			: mvFloatItemBase(parent, name, 1, default_value), m_min(minv), m_max(maxv), m_format(std::move(format)),
			m_vertical(vertical)
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
			}
			else
			{
				if (ImGui::SliderFloat(m_label.c_str(), m_value.data(), m_min, m_max, m_format.c_str()))
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

	private:

		float       m_min;
		float       m_max;
		std::string m_format;
		bool        m_vertical;

	};

	//-----------------------------------------------------------------------------
	// mvSliderInt
	//-----------------------------------------------------------------------------
	class mvSliderInt : public mvIntItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::SliderInt)

		mvSliderInt(const std::string& parent, const std::string& name, int default_value = 0, int minv = 0,
				int maxv = 100, std::string  format = "%d", bool vertical = false)
			: mvIntItemBase(parent, name, 1, default_value), m_min(minv), m_max(maxv), m_format(std::move(format)), m_vertical(vertical)
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
			}
			else
			{
				if (ImGui::SliderInt(m_label.c_str(), m_value.data(), m_min, m_max, m_format.c_str()))
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

	private:

		int         m_min;
		int         m_max;
		std::string m_format;
		bool        m_vertical;

	};

	//-----------------------------------------------------------------------------
	// mvSliderFloatMulti
	//-----------------------------------------------------------------------------
	template<mvAppItemType AppItemType, int num, ImGuiSlideCommand<float> imguicommand, typename T>
	class mvSliderFloatMulti : public mvFloatItemBase
	{

	public:

		MV_APPITEM_TYPE(AppItemType)

		mvSliderFloatMulti(const std::string& parent, const std::string& name, T* default_value, T minv, T maxv, std::string  format)
			: mvFloatItemBase(parent, name, num, default_value[0], default_value[1], default_value[2], default_value[3]),
			m_min(minv), m_max(maxv), m_format(std::move(format))
		{
		}

		void draw() override
		{
			if (imguicommand(m_label.c_str(), m_value.data(), m_min, m_max, m_format.c_str(), 0))
			{
				if (!m_dataSource.empty())
					mvDataStorage::AddData(m_dataSource, getPyValue());

				mvApp::GetApp()->runCallback(m_callback, m_name);

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());
			}
		}

	private:

		T       m_min;
		T       m_max;
		std::string m_format;

	};

	//-----------------------------------------------------------------------------
	// mvSliderIntMulti
	//-----------------------------------------------------------------------------
	template<mvAppItemType AppItemType, int num, ImGuiSlideCommand<int> imguicommand, typename T>
	class mvSliderIntMulti : public mvIntItemBase
	{

	public:

		MV_APPITEM_TYPE(AppItemType)

		mvSliderIntMulti(const std::string& parent, const std::string& name, T* default_value, T minv, T maxv, std::string  format)
			: mvIntItemBase(parent, name, num, default_value[0], default_value[1], default_value[2], default_value[3]),
			m_min(minv), m_max(maxv), m_format(std::move(format))
		{
		}

		void draw() override
		{
			if (imguicommand(m_label.c_str(), m_value.data(), m_min, m_max, m_format.c_str(), 0))
			{
				if (!m_dataSource.empty())
					mvDataStorage::AddData(m_dataSource, getPyValue());

				mvApp::GetApp()->runCallback(m_callback, m_name);

				// Context Menu
				if (!getPopup().empty())
					ImGui::OpenPopup(getPopup().c_str());
			}
		}

	private:

		T       m_min;
		T       m_max;
		std::string m_format;

	};

}