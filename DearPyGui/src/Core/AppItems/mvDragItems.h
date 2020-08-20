#pragma once

#include <utility>
#include <functional>

#include "Core/AppItems/mvTypeBases.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvDragFloat
//     * mvDragInt
//
//-----------------------------------------------------------------------------

namespace Marvel {

	template<typename T>
	using ImGuiDragCommand = bool(*)(const char*, T*, float, T, T, const char*, ImGuiSliderFlags);

	//-----------------------------------------------------------------------------
	// mvDragFloat
	//-----------------------------------------------------------------------------
	template<mvAppItemType AppItemType, int num, ImGuiDragCommand<float> imguicommand >
	class mvDragFloat : public mvFloatItemBase
	{

	public:

		MV_APPITEM_TYPE(AppItemType)

		mvDragFloat(const std::string& parent, const std::string& name, float* default_value, float speed = 1.0f,
              float minv = 0.0f, float maxv = 1.0f, std::string format = "%.3f")
			: mvFloatItemBase(parent, name, num, default_value[0], default_value[1], default_value[2], default_value[3]), 
			m_speed(speed), m_min(minv), m_max(maxv), m_format(std::move(format))
		{
		}

		void draw() override
		{
			if (imguicommand(m_label.c_str(), m_value.data(), m_speed, m_min, m_max, m_format.c_str(), 0))
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

		float       m_speed;
		float       m_min;
		float       m_max;
		std::string m_format;

	};

	//-----------------------------------------------------------------------------
	// mvDragInt
	//-----------------------------------------------------------------------------
	template<mvAppItemType AppItemType, int num, ImGuiDragCommand<int> imguicommand >
	class mvDragInt : public mvIntItemBase
	{

	public:

		MV_APPITEM_TYPE(AppItemType)

		mvDragInt(const std::string& parent, const std::string& name, int* default_value, float speed = 1.0f,
				int minv = 0, float maxv = 100, std::string format = "%d")
			: mvIntItemBase(parent, name, num, default_value[0], default_value[1], default_value[2], default_value[3]),
			m_speed(speed), m_min(minv), m_max(maxv), m_format(std::move(format))
		{
		}

		void draw() override
		{
			if (imguicommand(m_label.c_str(), m_value.data(), m_speed, m_min, m_max, m_format.c_str(), 0))
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

		float       m_speed;
		int         m_min;
		int         m_max;
		std::string m_format;

	};

}