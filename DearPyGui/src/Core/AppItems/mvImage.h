#pragma once

#include "mvAppItem.h"
#include "Core/mvTextureStorage.h"
#include "Core/mvPythonExceptions.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvImage
//
//-----------------------------------------------------------------------------

namespace Marvel {

	class mvImage : public mvAppItem
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::InputInt)

		mvImage(const std::string& parent, const std::string& name, const std::string& default_value, mvColor tint=MV_DEFAULT_COLOR,
			mvColor border = MV_DEFAULT_COLOR, const mvVec2& uv_min = { 0, 0 }, const mvVec2& uv_max = { 1, 1 },
			const std::string& secondaryDataSource = "")
			: mvAppItem(parent, name) , m_value(default_value), m_uv_min(uv_min), m_uv_max(uv_max), m_tintColor(tint), m_borderColor(border),
			m_secondaryDataSource(secondaryDataSource)
		{
		}

		virtual ~mvImage()
		{
			mvTextureStorage::DecrementTexture(m_value);
		}

		virtual void draw() override
		{

			if (m_texture == nullptr && !m_value.empty())
			{
				mvTextureStorage::AddTexture(m_value);
				mvTexture* texture = mvTextureStorage::GetTexture(m_value);
				if(texture == nullptr)
                {
                    PyObject* ex = PyErr_Format(PyExc_Exception,
                                                "Image %s could not be found for add_image. Check the path to the image "
                                                "you provided.", m_value.c_str());
                    PyErr_Print();
                    m_value = "";
                    return;
                }
				if (m_width == 0) m_width = texture->width*(m_uv_max.x - m_uv_min.x);
				if (m_height == 0) m_height = texture->height * (m_uv_max.y - m_uv_min.y);

				m_texture = texture->texture;

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
				PyObject* data = mvDataStorage::GetData(name);
				if (data == nullptr)
					return;

				auto floats = mvPythonTranslator::ToFloatVect(data);
				m_uv_min.x = floats[0];
				m_uv_min.y = floats[1];
				m_uv_max.x = floats[2];
				m_uv_max.y = floats[3];

				if (m_texture)
				{
					m_width = mvTextureStorage::GetTexture(m_value)->width * (m_uv_max.x - m_uv_min.x);
					m_height = mvTextureStorage::GetTexture(m_value)->height * (m_uv_max.y - m_uv_min.y);
				}
			}
		}

		virtual void setPyValue(PyObject* value) override
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
				m_width = mvTextureStorage::GetTexture(m_value)->width * (m_uv_max.x - m_uv_min.x);
				m_height = mvTextureStorage::GetTexture(m_value)->height * (m_uv_max.y - m_uv_min.y);
			}
		}

		virtual PyObject* getPyValue() const override
		{
			PyGILState_STATE gstate = PyGILState_Ensure();

			PyObject* pvalue = Py_BuildValue("s", m_value.c_str());

			PyGILState_Release(gstate);
			return pvalue;
		}

		inline void setValue(const std::string& value) { m_value = value; }
		inline const std::string& getValue() const { return m_value; }

	private:

		std::string m_value;
		mvVec2	    m_uv_min;
		mvVec2	    m_uv_max;
		mvColor     m_tintColor;
		mvColor     m_borderColor;
		std::string m_secondaryDataSource;
		void*       m_texture = nullptr;

	};

}