#pragma once

#include "Core/AppItems/mvTypeBases.h"

namespace Marvel {

	class mvSameLine : public mvNoneItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::SameLine)

		mvSameLine(const std::string& parent, float xoffset, float spacing)
			: mvNoneItemBase(parent, "SameLine"), m_xoffset(xoffset), m_spacing(spacing)
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