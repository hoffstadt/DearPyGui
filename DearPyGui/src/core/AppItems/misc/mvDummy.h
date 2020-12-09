#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	class mvDummy : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Dummy, "add_dummy")

		mvDummy(const std::string& name)
			: mvAppItem(name)
		{
			m_description.duplicatesAllowed = true;
		}

		void draw() override
		{
			ImGui::Dummy({ (float)m_width, (float)m_height });
		}

	};

}