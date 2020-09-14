#pragma once

#include <utility>

#include "mvAppItem.h"
#include "Core/mvTextureStorage.h"
#include "Core/mvPythonExceptions.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvImage
//     * mvImageButton
//
//-----------------------------------------------------------------------------

namespace Marvel {

	//-----------------------------------------------------------------------------
	// mvImage
	//-----------------------------------------------------------------------------
	class mvImage : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::InputInt)

		mvImage(const std::string& parent, const std::string& name, std::string  default_value, mvColor tint=MV_DEFAULT_COLOR,
			mvColor border = MV_DEFAULT_COLOR, const mvVec2& uv_min = { 0, 0 }, const mvVec2& uv_max = { 1, 1 },
			std::string  secondaryDataSource = "")
			: mvAppItem(parent, name) , m_value(std::move(default_value)), m_uv_min(uv_min), m_uv_max(uv_max), m_tintColor(tint), m_borderColor(border),
			m_secondaryDataSource(std::move(secondaryDataSource))
		{
		}

		~mvImage() override
		{
			mvTextureStorage::DecrementTexture(m_value);
		}

		void draw() override
		{

			if (m_texture == nullptr && !m_value.empty())
			{
				mvTextureStorage::AddTexture(m_value);
				mvTexture* texture = mvTextureStorage::GetTexture(m_value);
				if(texture == nullptr)
                {
                    PyErr_Format(PyExc_Exception,
                                                "Image %s could not be found for add_image. Check the path to the image "
                                                "you provided.", m_value.c_str());
                    PyErr_Print();
                    m_value = "";
                    return;
                }
				if (m_width == 0) m_width = (int)((float)texture->width*(m_uv_max.x - m_uv_min.x));
				if (m_height == 0) m_height = (int)((float)texture->height * (m_uv_max.y - m_uv_min.y));

				m_texture = texture->texture;

			}

			if (m_texture)
			{
				ImGui::Image(m_texture, ImVec2((float)m_width, (float)m_height), ImVec2(m_uv_min.x, m_uv_min.y), ImVec2(m_uv_max.x, m_uv_max.y),
					ImVec4((float)m_tintColor.r, (float)m_tintColor.g, (float)m_tintColor.b, (float)m_tintColor.a),
					ImVec4((float)m_borderColor.r, (float)m_borderColor.g, (float)m_borderColor.b, (float)m_borderColor.a));

				// Regular Tooltip (simple)
				if (!getTip().empty() && ImGui::IsItemHovered())
					ImGui::SetTooltip("%s", getTip().c_str());
			}

		}

		void updateData(const std::string& name) override
		{
			if (name == m_secondaryDataSource)
			{
				PyObject* data = mvDataStorage::GetData(name);
				if (data == nullptr)
					return;

				auto floats = ToFloatVect(data);
				m_uv_min.x = floats[0];
				m_uv_min.y = floats[1];
				m_uv_max.x = floats[2];
				m_uv_max.y = floats[3];

				if (m_texture)
				{
					m_width = (int)((float)mvTextureStorage::GetTexture(m_value)->width * (m_uv_max.x - m_uv_min.x));
					m_height = (int)((float)mvTextureStorage::GetTexture(m_value)->height * (m_uv_max.y - m_uv_min.y));
				}
			}
		}

		void setPyValue(PyObject* value) override
		{

			std::string oldvalue = m_value;

			PyGILState_STATE gstate = PyGILState_Ensure();

			if (!PyUnicode_Check(value))
			{
				PyGILState_Release(gstate);
				ThrowPythonException(m_name + " type must be a string.");
				return;
			}

			m_value = PyUnicode_AsUTF8(value);
			PyGILState_Release(gstate);

			// clean up old resource
			if (!m_value.empty() && oldvalue != m_value)
			{
				mvTextureStorage::DecrementTexture(oldvalue);
				mvTextureStorage::AddTexture(m_value);
			}

			m_texture = mvTextureStorage::GetTexture(m_value);

			if (m_texture)
			{
				m_width = (int)((float)mvTextureStorage::GetTexture(m_value)->width * (m_uv_max.x - m_uv_min.x));
				m_height = (int)((float)mvTextureStorage::GetTexture(m_value)->height * (m_uv_max.y - m_uv_min.y));
			}
		}

		[[nodiscard]] PyObject* getPyValue() const override
		{
			PyGILState_STATE gstate = PyGILState_Ensure();

			PyObject* pvalue = Py_BuildValue("s", m_value.c_str());

			PyGILState_Release(gstate);
			return pvalue;
		}

		void                             setValue(const std::string& value) { m_value = value; }
		[[nodiscard]] const std::string& getValue() const { return m_value; }

	private:

		std::string m_value;
		mvVec2	    m_uv_min;
		mvVec2	    m_uv_max;
		mvColor     m_tintColor;
		mvColor     m_borderColor;
		std::string m_secondaryDataSource;
		void*       m_texture = nullptr;

	};

	//-----------------------------------------------------------------------------
	// mvImageButton
	//-----------------------------------------------------------------------------
	class mvImageButton : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::ImageButton)

		mvImageButton(const std::string& parent, const std::string& name, std::string  default_value, mvColor tint,
				mvColor backgroundColor, const mvVec2& uv_min, const mvVec2& uv_max, int framePadding)
			: mvAppItem(parent, name), m_value(std::move(default_value)), m_uv_min(uv_min), m_uv_max(uv_max), m_tintColor(tint), 
			m_backgroundColor(backgroundColor), m_framePadding(framePadding)
		{
		}

		~mvImageButton() override
		{
			mvTextureStorage::DecrementTexture(m_value);
		}

		void draw() override
		{

			if (m_texture == nullptr && !m_value.empty())
			{
				mvTextureStorage::AddTexture(m_value);
				mvTexture* texture = mvTextureStorage::GetTexture(m_value);
				if (texture == nullptr)
				{
					PyErr_Format(PyExc_Exception,
						"Image %s could not be found for add_image. Check the path to the image "
						"you provided.", m_value.c_str());
					PyErr_Print();
					m_value = "";
					return;
				}
				if (m_width == 0) m_width = (int)((float)texture->width * (m_uv_max.x - m_uv_min.x));
				if (m_height == 0) m_height = (int)((float)texture->height * (m_uv_max.y - m_uv_min.y));

				m_texture = texture->texture;

			}

			if (m_texture)
			{
				//if (ImGui::ImageButton(m_texture, ImVec2((float)m_width, (float)m_height),
				//	ImVec2(m_uv_min.x, m_uv_min.y), ImVec2(m_uv_max.x, m_uv_max.y), m_framePadding,
				//	m_backgroundColor.toVec4(), m_tintColor.toVec4()))
				if (ImGui::ImageButton(m_texture, ImVec2((float)m_width, (float)m_height),
					ImVec2(m_uv_min.x, m_uv_min.y), ImVec2(m_uv_max.x, m_uv_max.y), m_framePadding,
					m_backgroundColor.toVec4(), m_tintColor.toVec4()))
				{
					mvApp::GetApp()->runCallback(m_callback, m_name, m_callbackData);

						// Context Menu
						if (!getPopup().empty())
							ImGui::OpenPopup(getPopup().c_str());
				}

				// Regular Tooltip (simple)
				if (!getTip().empty() && ImGui::IsItemHovered())
					ImGui::SetTooltip("%s", getTip().c_str());
			}

		}

	private:

		std::string m_value;
		mvVec2	    m_uv_min;
		mvVec2	    m_uv_max;
		mvColor     m_tintColor;
		mvColor     m_backgroundColor;
		void*       m_texture = nullptr;
		int         m_framePadding;

	};

}