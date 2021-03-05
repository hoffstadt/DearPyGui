#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	class mvSpacing : public mvIntPtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Spacing, mvSpacing, "add_spacing")

		MV_START_COLOR_CONSTANTS
		MV_END_COLOR_CONSTANTS

		MV_START_STYLE_CONSTANTS
		MV_END_STYLE_CONSTANTS

		mvSpacing(const std::string& name, int count)
			: mvIntPtrBase(name, count)
		{
			m_description.duplicatesAllowed = true;
		}

		void draw() override
		{
			for (int i = 0; i < *m_value; i++)
				ImGui::Spacing();
		}

	};

}