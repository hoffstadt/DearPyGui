#pragma once

#include "Core/AppItems/mvTypeBases.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvSliderFloat
//     * mvSliderFloat4
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvSliderFloat
	//-----------------------------------------------------------------------------
	class mvSliderFloat : public mvFloatItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::SliderFloat)

		mvSliderFloat(const std::string& parent, const std::string& name, float default_value=0.0f, float minv=0.0f, 
			float maxv=1.0f, const std::string& format="%.3f", float power = 1.0f, bool vertical = false)
			: mvFloatItemBase(parent, name, 1, default_value), m_min(minv), m_max(maxv), m_format(format), 
			m_power(power), m_vertical(vertical)
		{
		}

		virtual void draw() override
		{
			if (m_vertical)
			{
				if (m_height < 1.0f)
					m_height = 100.f;
				if (m_width < 1.0f)
					m_width = 20.f;

				if (ImGui::VSliderFloat(m_label.c_str(), ImVec2(m_width, m_height), m_value, m_min, m_max, m_format.c_str(), m_power))
				{
					auto threadpool = mvThreadPool::GetThreadPool();
					threadpool->submit(std::bind(&mvApp::triggerCallback, mvApp::GetApp(), m_callback, m_name));

					// Context Menu
					if (getPopup() != "")
						ImGui::OpenPopup(getPopup().c_str());
				}
			}
			else
			{
				if (ImGui::SliderFloat(m_label.c_str(), m_value, m_min, m_max, m_format.c_str(), m_power))
				{
					auto threadpool = mvThreadPool::GetThreadPool();
					threadpool->submit(std::bind(&mvApp::triggerCallback, mvApp::GetApp(), m_callback, m_name));

					// Context Menu
					if (getPopup() != "")
						ImGui::OpenPopup(getPopup().c_str());
				}
			}
		}

	private:

		float       m_min;
		float       m_max;
		std::string m_format;
		float       m_power;
		bool        m_vertical;

	};

	//-----------------------------------------------------------------------------
	// mvSliderFloat4
	//-----------------------------------------------------------------------------
	class mvSliderFloat4 : public mvFloatItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::SliderFloat4)

		mvSliderFloat4(const std::string& parent, const std::string& name, float default_value[4], float minv = 0.0f,
				float maxv = 1.0f, const std::string& format = "%.3f", float power = 1.0f)
			: mvFloatItemBase(parent, name, 4, default_value[0], default_value[1], default_value[2], default_value[3]), 
			m_min(minv), m_max(maxv), m_format(format), m_power(power)
		{
		}

		virtual void draw() override
		{
			if (ImGui::SliderFloat4(m_label.c_str(), m_value, m_min, m_max, m_format.c_str(), m_power))
			{
				auto threadpool = mvThreadPool::GetThreadPool();
				threadpool->submit(std::bind(&mvApp::triggerCallback, mvApp::GetApp(), m_callback, m_name));

				// Context Menu
				if (getPopup() != "")
					ImGui::OpenPopup(getPopup().c_str());
			}
		}

	private:

		float       m_min;
		float       m_max;
		std::string m_format;
		float       m_power;

	};

	//-----------------------------------------------------------------------------
	// mvIntFloat
	//-----------------------------------------------------------------------------
	class mvSliderInt : public mvIntItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::SliderInt)

		mvSliderInt(const std::string& parent, const std::string& name, int default_value = 0, int minv = 0,
				int maxv = 100, const std::string& format = "%d", bool vertical = false)
			: mvIntItemBase(parent, name, 1, default_value), m_min(minv), m_max(maxv), m_format(format), m_vertical(vertical)
		{
		}

		virtual void draw() override
		{
			if (m_vertical)
			{
				if (m_height < 1.0f)
					m_height = 100.f;
				if (m_width < 1.0f)
					m_width = 20.f;

				if (ImGui::VSliderInt(m_label.c_str(), ImVec2(m_width, m_height), m_value, m_min, m_max, m_format.c_str()))
				{
					auto threadpool = mvThreadPool::GetThreadPool();
					threadpool->submit(std::bind(&mvApp::triggerCallback, mvApp::GetApp(), m_callback, m_name));

					// Context Menu
					if (getPopup() != "")
						ImGui::OpenPopup(getPopup().c_str());
				}
			}
			else
			{
				if (ImGui::SliderInt(m_label.c_str(), m_value, m_min, m_max, m_format.c_str()))
				{
					mvApp::GetApp()->triggerCallback(m_callback, m_name);

					// Context Menu
					if (getPopup() != "")
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
	// mvSliderInt4
	//-----------------------------------------------------------------------------
	class mvSliderInt4 : public mvIntItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::SliderInt4)

		mvSliderInt4(const std::string& parent, const std::string& name, int default_value[4], int minv = 0,
				int maxv = 100, const std::string& format = "%d")
			: mvIntItemBase(parent, name, 4, default_value[0], default_value[1], default_value[2], default_value[3]),
			m_min(minv), m_max(maxv), m_format(format)
		{
		}

		virtual void draw() override
		{
			if (ImGui::SliderInt4(m_label.c_str(), m_value, m_min, m_max, m_format.c_str()))
			{
				auto threadpool = mvThreadPool::GetThreadPool();
				threadpool->submit(std::bind(&mvApp::triggerCallback, mvApp::GetApp(), m_callback, m_name));

				// Context Menu
				if (getPopup() != "")
					ImGui::OpenPopup(getPopup().c_str());
			}
		}

	private:

		int         m_min;
		int         m_max;
		std::string m_format;

	};

}