#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	class mvSpacing : public mvIntPtrBase
	{

	public:

		MV_APPITEM_TYPE_OLD_SYSTEM(mvAppItemType::Spacing, "add_spacing")

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