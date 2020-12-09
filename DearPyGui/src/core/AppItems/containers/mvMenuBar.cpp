#pragma once

#include "mvMenuBar.h"
#include "mvApp.h"

namespace Marvel {


	mvMenuBar::mvMenuBar(const std::string& name)
			: mvBoolPtrBase(name, true, name)
		{
			m_description.container = true;

			// TODO use code below to set item height when font and scale systems are done
			//float FontSize = ImGui::GetFontSize(); // = Base Font Size * Current Window Scale
			//ImGuiStyle currentStyle = ImGui::GetStyle(); // = Padding for the Top and Bottom
			//m_height = int(currentStyle.FramePadding.y * 2 + FontSize);
			m_height = 21;
		}

	void mvMenuBar::draw()
	{
		auto styleManager = m_styleManager.getScopedStyleManager();

		if (ImGui::BeginMenuBar())
		{
			for (mvAppItem* item : m_children)
			{
				// skip item if it's not shown
				if (!item->isShown())
					continue;

				// set item width
				if (item->getWidth() != 0)
					ImGui::SetNextItemWidth((float)item->getWidth());

				item->draw();

				// Regular Tooltip (simple)
				if (!item->getTip().empty() && ImGui::IsItemHovered())
					ImGui::SetTooltip("%s", item->getTip().c_str());

				item->getState().update();
			}
			ImGui::EndMenuBar();
		}
	}

}