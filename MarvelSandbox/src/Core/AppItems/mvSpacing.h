#pragma once

#include "Core/AppItems/mvTypeBases.h"

namespace Marvel {

	class mvSpacing : public mvNoneItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Spacing)

		mvSpacing(const std::string& parent, int count)
			: mvNoneItemBase(parent, "Spacing")
		{
			m_value = count;
		}

		virtual void draw() override
		{
			for (int i = 0; i < m_value; i++)
				ImGui::Spacing();
		}

		inline int getValue() const { return m_value; }

	private:

		int m_value;

	};

}