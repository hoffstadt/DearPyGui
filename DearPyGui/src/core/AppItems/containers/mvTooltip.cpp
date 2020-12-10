#include "mvTooltip.h"

namespace Marvel {

	mvTooltip::mvTooltip(const std::string& name)
		: 
		mvBoolPtrBase(name, false, name)
	{

		// has to be showed that way it can check for hovering
		// otherwise it will never show
		m_show = true;
		m_description.container = true;
		
	}

	void mvTooltip::draw()
	{
		if (ImGui::IsItemHovered())
		{
			auto styleManager = m_styleManager.getScopedStyleManager();
			ImGui::BeginTooltip();
			for (mvAppItem* item : m_children)
			{
				// skip item if it's not shown
				if (!item->m_show)
					continue;

				// set item width
				if (item->m_width != 0)
					ImGui::SetNextItemWidth((float)item->m_width);

				item->draw();

				// Regular Tooltip (simple)
				if (!item->m_tip.empty() && ImGui::IsItemHovered())
					ImGui::SetTooltip("%s", item->m_tip.c_str());

				item->getState().update();
			}

			ImGui::EndTooltip();
		}

	}

}