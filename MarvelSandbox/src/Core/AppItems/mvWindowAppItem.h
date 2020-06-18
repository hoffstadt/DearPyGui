#pragma once

#include "Core/AppItems/mvTypeBases.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvWindowAppitem
	//-----------------------------------------------------------------------------
	class mvWindowAppitem : public mvNoneItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Window)

		mvWindowAppitem(const std::string& parent, const std::string& name, int width, int height)
			: mvNoneItemBase(parent, name)
		{
			m_width = width;
			m_height = height;
		}

		virtual void draw() override
		{

			ImGui::SetNextWindowSize(ImVec2(m_width, m_height), ImGuiCond_FirstUseEver);

			if (ImGui::Begin(m_label.c_str(), &m_show, m_windowflags))
			{
				for (mvAppItem* item : m_children)
				{
					// skip item if it's not shown
					if (!item->isShown())
						continue;

					// set item width
					if (item->getWidth() > 0)
						ImGui::SetNextItemWidth((float)item->getWidth());

					item->pushColorStyles();
					item->draw();
					item->popColorStyles();

					// Regular Tooltip (simple)
					if (item->getTip() != "" && ImGui::IsItemHovered())
						ImGui::SetTooltip(item->getTip().c_str());
				}
				ImGui::End();
			}

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
			m_show = true;
		}

		virtual void draw() override
		{
			mvApp::GetApp()->popParent();
			ImGui::End();
		}

	};

}