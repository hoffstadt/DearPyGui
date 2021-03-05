#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	class mvDummy : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Dummy, mvDummy, "add_dummy")

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

		mvDummy(const std::string& name)
			: mvAppItem(name)
		{
			m_description.duplicatesAllowed = true;
		}

		void draw() override
		{
			ImGui::Dummy({ (float)m_core_config.width, (float)m_core_config.height });
		}

	};

}