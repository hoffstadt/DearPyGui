#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	class mvSeparator : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Separator, "add_separator")

			mvSeparator(const std::string& name)
			: mvAppItem(name)
		{
			m_description.duplicatesAllowed = true;
		}

		void draw() override
		{
			auto styleManager = m_styleManager.getScopedStyleManager();

			ImGui::Separator();

		}

	};

}