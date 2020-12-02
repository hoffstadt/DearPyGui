#include "mvTooltip.h"

namespace Marvel {

	mvTooltip::mvTooltip(const std::string& name)
		: 
		mvPtrBase<bool, 1>(name, false, name)
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
				if (!item->isShown())
					continue;

				// set item width
				if (item->getWidth() > 0)
					ImGui::SetNextItemWidth((float)item->getWidth());

				item->draw();

				// Regular Tooltip (simple)
				if (!item->getTip().empty() && ImGui::IsItemHovered())
					ImGui::SetTooltip("%s", item->getTip().c_str());

				item->getState().update();
			}

			ImGui::EndTooltip();
		}

	}

}