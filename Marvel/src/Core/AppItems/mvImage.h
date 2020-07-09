#pragma once

#include "Core/AppItems/mvAppItem.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvImage
//
//-----------------------------------------------------------------------------

namespace Marvel {

	class mvImage : public mvImageItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::InputInt)

		mvImage(const std::string& parent, const std::string& name, const std::string& default_value, mvColor tint=MV_DEFAULT_COLOR,
			mvColor border = MV_DEFAULT_COLOR)
			: mvImageItemBase(parent, name, default_value) , m_tintColor(tint), m_borderColor(border)
		{
		}

		virtual void draw() override
		{

			if (m_texture == nullptr && !m_value.empty())
			{
				LoadTextureFromFile(m_value.c_str(), &m_texture, &m_picWidth, &m_picHeight);
				if (m_width == 0) m_width = m_picWidth;
				if (m_height == 0) m_height = m_picHeight;
			}

			if(m_texture)
				ImGui::Image(m_texture, ImVec2(m_width, m_height), ImVec2(0,0), ImVec2(1,1), 
					ImVec4(m_tintColor.r, m_tintColor.g, m_tintColor.b, m_tintColor.a),
					ImVec4(m_borderColor.r, m_borderColor.g, m_borderColor.b, m_borderColor.a));

			// Context Menu
			if (getPopup() != "")
				ImGui::OpenPopup(getPopup().c_str());

		}

	private:

		mvColor m_tintColor;
		mvColor m_borderColor;
		int     m_picWidth = 0;
		int     m_picHeight = 0;

	};

}