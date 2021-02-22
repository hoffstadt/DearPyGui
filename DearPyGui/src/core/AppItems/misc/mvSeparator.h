#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	class mvSeparator : public mvAppItem
	{
		MV_APPITEM_TYPE(mvAppItemType::Separator, mvSeparator, "add_separator")

		MV_CREATE_THEME_CONSTANT(mvAppItemType::Separator, mvThemeCol_Separator, 27L, 0L);

		MV_START_COLOR_CONSTANTS
			MV_CREATE_CONSTANT_PAIR(mvThemeCol_Separator),
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

	public:

			mvSeparator(const std::string& name)
			: mvAppItem(name)
		{
			m_description.duplicatesAllowed = true;
		}

		void draw() override
		{
			auto styleManager = m_styleManager.getScopedStyleManager();
			mvImGuiThemeScope scope(this);

			ImGui::Separator();

		}

	};

}