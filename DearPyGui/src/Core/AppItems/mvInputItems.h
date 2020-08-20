#pragma once

#include <utility>
#include "mvAppItem.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvInputInt
//     * mvInputIntMulti
//     * mvInputFloatMulti
//
//-----------------------------------------------------------------------------

namespace Marvel {

	using ImGuiIntCommand   = bool(*)(const char*, int*, ImGuiInputTextFlags);
	using ImGuiFloatCommand = bool(*)(const char*, float*, const char*, ImGuiInputTextFlags);

	//-----------------------------------------------------------------------------
	// mvInputInt
	//-----------------------------------------------------------------------------
	class mvInputInt : public mvIntItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::InputInt)

			mvInputInt(const std::string& parent, const std::string& name, int default_value)
			: mvIntItemBase(parent, name, 1, default_value) {}

		void draw() override
		{

			if (ImGui::InputInt(m_label.c_str(), m_value.data()))
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
	// mvInputIntMulti
	//-----------------------------------------------------------------------------
	template<mvAppItemType AppItemType, int num, ImGuiIntCommand imguicommand>
	class mvInputIntMulti : public mvIntItemBase
	{

	public:

		MV_APPITEM_TYPE(AppItemType)

		mvInputIntMulti(const std::string& parent, const std::string& name, int default_value[2])
			: mvIntItemBase(parent, name, num, default_value[0], default_value[1], default_value[2], default_value[3]) {}

		void draw() override
		{

			if (imguicommand(m_label.c_str(), m_value.data(), 0))
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
	// mvInputFloat
	//-----------------------------------------------------------------------------
	class mvInputFloat : public mvFloatItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::InputFloat)

		mvInputFloat(const std::string& parent, const std::string& name, float default_value, std::string  format = "%.3f")
			: mvFloatItemBase(parent, name, 1, default_value), m_format(std::move(format))
		{
		}

		void draw() override
		{
			if (ImGui::InputFloat(m_label.c_str(), m_value.data(), 0.0f, 0.0f, m_format.c_str()))
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

		std::string m_format;

	};


	//-----------------------------------------------------------------------------
	// mvInputFloatMulti
	//-----------------------------------------------------------------------------
	template<mvAppItemType AppItemType, int num, ImGuiFloatCommand imguicommand>
	class mvInputFloatMulti : public mvFloatItemBase
	{

	public:

		MV_APPITEM_TYPE(AppItemType)

		mvInputFloatMulti(const std::string& parent, const std::string& name, float* default_value, std::string  format = "%.3f")
			: mvFloatItemBase(parent, name, num, default_value[0], default_value[1], default_value[2], default_value[3]), m_format(std::move(format))
		{
		}

		void draw() override
		{
			if (imguicommand(m_label.c_str(), m_value.data(), m_format.c_str(), 0))
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

		std::string m_format;

	};

}