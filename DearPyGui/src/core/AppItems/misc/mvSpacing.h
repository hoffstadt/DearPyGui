#pragma once

#include "mvTypeBases.h"

namespace Marvel {

	class mvSpacing : public mvIntPtrBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Spacing, "add_spacing")

			mvSpacing(const std::string& name, int count)
			: mvIntPtrBase(name, count, name)
		{
			m_description.duplicatesAllowed = true;
		}

		void draw() override
		{
			for (int i = 0; i < m_value[0]; i++)
				ImGui::Spacing();
		}

	};

}