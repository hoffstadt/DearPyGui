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
			mvColor border = MV_DEFAULT_COLOR, const mvVec2& uv_min = { 0, 0 }, const mvVec2& uv_max = { 1, 1 },
			const std::string& secondaryDataSource = "")
			: mvImageItemBase(parent, name, default_value) , m_tintColor(tint), m_borderColor(border), m_uv_min(uv_min), m_uv_max(uv_max)
		{
		}

		virtual void draw() override
		{

			if (m_texture == nullptr && !m_value.empty())
			{
				updateTexture();
				auto& textures = mvApp::GetApp()->getTextures();
				if (m_width == 0) m_width = textures[m_value].width*(m_uv_max.x - m_uv_min.x);
				if (m_height == 0) m_height = textures[m_value].height * (m_uv_max.y - m_uv_min.y);
			}

			if(m_texture)
				ImGui::Image(m_texture, ImVec2(m_width, m_height), ImVec2(m_uv_min.x,m_uv_min.y), ImVec2(m_uv_max.x, m_uv_max.y),
					ImVec4(m_tintColor.r, m_tintColor.g, m_tintColor.b, m_tintColor.a),
					ImVec4(m_borderColor.r, m_borderColor.g, m_borderColor.b, m_borderColor.a));

			// Context Menu
			if (getPopup() != "")
				ImGui::OpenPopup(getPopup().c_str());

		}

		virtual void updateData(const std::string& name) override
		{
			if (name == m_secondaryDataSource)
			{
				PyObject* data = mvApp::GetApp()->getData(name);
				if (data == nullptr)
					return;

				auto floats = mvPythonTranslator::getFloatVec(data);
				m_uv_min.x = floats[0];
				m_uv_min.y = floats[1];
				m_uv_max.x = floats[2];
				m_uv_max.y = floats[3];
			}
		}

	private:

		mvColor     m_tintColor;
		mvColor     m_borderColor;
		mvVec2	    m_uv_min;
		mvVec2	    m_uv_max;
		std::string m_secondaryDataSource;

	};

}