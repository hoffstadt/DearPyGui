#pragma once

#include "Core/mvAppItem.h"

namespace Marvel {

	class mvSameLine : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::SameLine)
		MV_NORETURN_VALUE()

		mvSameLine(const std::string& parent, float xoffset, float spacing)
			: mvAppItem(parent, "SameLine"), m_xoffset(xoffset), m_spacing(spacing)
		{
		}

		virtual void draw() override
		{
			ImGui::SameLine(m_xoffset, m_spacing);
		}

	private:

		float m_xoffset;
		float m_spacing;

	};

}