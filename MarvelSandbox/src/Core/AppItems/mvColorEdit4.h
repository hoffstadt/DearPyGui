#pragma once

#include "Core/AppItems/mvTypeBases.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvColorEdit4
//
//-----------------------------------------------------------------------------

namespace Marvel {

	class mvColorEdit4 : public mvFloatItemBase
	{

	public:

		MV_APPITEM_TYPE(mvAppItemType::ColorEdit4)

		mvColorEdit4(const std::string& parent, const std::string& name, mvColor default_value)
			: mvFloatItemBase(parent, name, 4, default_value.r / 255.0, default_value.g / 255.0, default_value.b / 255.0, default_value.a / 255.0)
		{}

		virtual void setPyValue(PyObject* value) override
		{
			std::lock_guard<std::mutex> lock(m_wmutex);

			for (int i = 0; i < PyTuple_Size(value); i++)
				m_value[i] = (float)PyLong_AsLong(PyTuple_GetItem(value, i)) / 255.0;
		}

		virtual PyObject* getPyValue() const override
		{
			std::lock_guard<std::mutex> lock(m_rmutex);

			PyObject* value = PyTuple_New(m_valuecount);
			for (int i = 0; i < m_valuecount; i++)
				PyTuple_SetItem(value, i, PyLong_FromLong(m_value[i] * 255.0));
			return value;

		}

		virtual void draw() override
		{

			if (ImGui::ColorEdit4(m_label.c_str(), m_value))
			{
				auto threadpool = mvThreadPool::GetThreadPool();
				threadpool->submit(std::bind(&mvApp::triggerCallback, mvApp::GetApp(), m_callback, m_name));

				// Context Menu
				if (getPopup() != "")
					ImGui::OpenPopup(getPopup().c_str());
			}
		}

	};

}