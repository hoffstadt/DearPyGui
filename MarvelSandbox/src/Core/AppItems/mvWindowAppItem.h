#pragma once

#include "Core/AppItems/mvTypeBases.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvWindowAppitem
	//-----------------------------------------------------------------------------
	class mvWindowAppitem : public mvBoolItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Window)

			mvWindowAppitem(const std::string& parent, const std::string& name, int width, int height)
			: mvBoolItemBase(parent, name, false)
		{
			m_width = width;
			m_height = height;
		}

		virtual void draw() override
		{
			ImGui::End();

			ImGui::SetNextWindowSize(ImVec2(m_width, m_height), ImGuiCond_FirstUseEver);
			ImGui::Begin(m_label.c_str(), &m_show, m_windowflags);

			mvApp::GetApp()->pushParent(this);

			// set current child value true
			m_value = true;
		}

	private:

		int m_height;
		ImGuiWindowFlags m_windowflags = ImGuiWindowFlags_NoSavedSettings;


	};

	//-----------------------------------------------------------------------------
	// mvWindowAppitem
	//-----------------------------------------------------------------------------
	class mvEndWindowAppitem : public mvNoneItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::EndWindow)

		mvEndWindowAppitem(const std::string& parent)
			: mvNoneItemBase(parent, "EndWindow")
		{
		}

		virtual void draw() override
		{
			mvApp::GetApp()->popParent();
		}

	};

}