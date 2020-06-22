#pragma once

#include "Core/AppItems/mvTypeBases.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvDragFloat
//     * mvDragFloat4
//     * mvDragInt
//     * mvDragInt4
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvDragFloat
	//-----------------------------------------------------------------------------
	class mvDragFloat : public mvFloatItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::DragFloat)

		mvDragFloat(const std::string& parent, const std::string& name, float default_value = 0.0f, float speed = 1.0f, float minv = 0.0f,
				float maxv = 1.0f, const std::string& format = "%.3f", float power = 1.0f)
			: mvFloatItemBase(parent, name, 1, default_value), m_speed(speed), m_min(minv), m_max(maxv), m_format(format),
			m_power(power)
		{
		}

		virtual void draw() override
		{

			if (ImGui::DragFloat(m_label.c_str(), m_value, m_speed, m_min, m_max, m_format.c_str(), m_power))
			{
				auto threadpool = mvThreadPool::GetThreadPool();
				threadpool->submit(std::bind(&mvApp::triggerCallback, mvApp::GetApp(), m_callback, m_name));

				// Context Menu
				if (getPopup() != "")
					ImGui::OpenPopup(getPopup().c_str());
			}

		}

	private:

		float       m_speed;
		float       m_min;
		float       m_max;
		std::string m_format;
		float       m_power;

	};

	//-----------------------------------------------------------------------------
	// mvDragFloat4
	//-----------------------------------------------------------------------------
	class mvDragFloat4 : public mvFloatItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::DragFloat4)

		mvDragFloat4(const std::string& parent, const std::string& name, float default_value[4], float speed = 1.0f, float minv = 0.0f,
				float maxv = 1.0f, const std::string& format = "%.3f", float power = 1.0f)
			: mvFloatItemBase(parent, name, 4, default_value[0], default_value[1], default_value[2], default_value[3]), m_speed(speed),
			m_min(minv), m_max(maxv), m_format(format), m_power(power)
		{
		}

		virtual void draw() override
		{
			if (ImGui::DragFloat4(m_label.c_str(), m_value, m_speed, m_min, m_max, m_format.c_str(), m_power))
			{
				auto threadpool = mvThreadPool::GetThreadPool();
				threadpool->submit(std::bind(&mvApp::triggerCallback, mvApp::GetApp(), m_callback, m_name));

				// Context Menu
				if (getPopup() != "")
					ImGui::OpenPopup(getPopup().c_str());
			}
		}

	private:

		float       m_speed;
		float       m_min;
		float       m_max;
		std::string m_format;
		float       m_power;

	};

	//-----------------------------------------------------------------------------
	// mvDragInt
	//-----------------------------------------------------------------------------
	class mvDragInt : public mvIntItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::DragInt)

		mvDragInt(const std::string& parent, const std::string& name, int default_value = 0, float speed = 1.0f, int minv = 0,
				int maxv = 100, const std::string& format = "%d")
			: mvIntItemBase(parent, name, 1, default_value), m_speed(speed), m_min(minv), m_max(maxv), m_format(format)
		{
		}

		virtual void draw() override
		{

			if (ImGui::DragInt(m_label.c_str(), m_value, m_speed, m_min, m_max, m_format.c_str()))
			{
				mvApp::GetApp()->triggerCallback(m_callback, m_name);

				// Context Menu
				if (getPopup() != "")
					ImGui::OpenPopup(getPopup().c_str());
			}
		}

	private:

		float       m_speed;
		int         m_min;
		int         m_max;
		std::string m_format;
		bool        m_vertical;

	};

	//-----------------------------------------------------------------------------
	// mvSliderInt4
	//-----------------------------------------------------------------------------
	class mvDragInt4 : public mvIntItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::DragInt4)

		mvDragInt4(const std::string& parent, const std::string& name, int default_value[4], float speed = 1.0f, int minv = 0,
				int maxv = 100, const std::string& format = "%d")
			: mvIntItemBase(parent, name, 4, default_value[0], default_value[1], default_value[2], default_value[3]), m_speed(speed),
			m_min(minv), m_max(maxv), m_format(format)
		{
		}

		virtual void draw() override
		{
			if (ImGui::DragInt4(m_label.c_str(), m_value, m_speed, m_min, m_max, m_format.c_str()))
			{
				auto threadpool = mvThreadPool::GetThreadPool();
				threadpool->submit(std::bind(&mvApp::triggerCallback, mvApp::GetApp(), m_callback, m_name));

				// Context Menu
				if (getPopup() != "")
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