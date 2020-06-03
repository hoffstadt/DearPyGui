#pragma once

#include "Core/mvAppItem.h"

namespace Marvel {

	class mvSpacing : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::Spacing)
		MV_NORETURN_VALUE()

		mvSpacing(const std::string& parent, int count)
			: mvAppItem(parent, "Spacing")
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