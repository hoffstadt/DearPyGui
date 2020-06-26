#pragma once

#include "Core/AppItems/mvAppItem.h"

//-----------------------------------------------------------------------------
// Widget Index
//
//     * mvSimplePlot
//
//-----------------------------------------------------------------------------

namespace Marvel{

	class mvSimplePlot : public mvAppItem
	{

		MV_APPITEM_TYPE(mvAppItemType::SimplePlot)

	public:
		mvSimplePlot(const std::string& parent, const std::string& name, const std::vector<float> value, const std::string& overlay, float scale_min, 
			float scale_max, float height, bool histogram)
			: mvAppItem(parent, name), m_value(value), m_overlay(overlay), m_min(scale_min), m_max(scale_max), m_height(height), m_histogram(histogram)
		{
		}

		virtual void draw() override
		{

			if(m_histogram)
				ImGui::PlotHistogram(m_label.c_str(), m_value.data(), m_value.size(), 0, m_overlay.c_str(), 
					m_min, m_max, ImVec2(m_width, m_height));
			else
				ImGui::PlotLines(m_label.c_str(), m_value.data(), m_value.size(), 0, m_overlay.c_str(), 
					m_min, m_max, ImVec2(m_width, m_height));
		}

		virtual void setPyValue(PyObject* value) override
		{
			std::vector<float> newvalue;

			for (int i = 0; i < PyTuple_Size(value); i++)
				newvalue.push_back((float)PyFloat_AsDouble(PyTuple_GetItem(value, i)));

			m_value = newvalue;
		}

		virtual PyObject* getPyValue() const override
		{
			PyObject* value = PyTuple_New(m_value.size());
			for (int i = 0; i < m_value.size(); i++)
				PyTuple_SetItem(value, i, PyFloat_FromDouble(m_value[i]));
			return value;
		}

		inline void setValue(const std::vector<float>& value){ m_value = value;}
		inline const std::vector<float>& getValue() const { return m_value; }

	private:

		std::vector<float> m_value;
		std::string        m_overlay;
		float              m_min;
		float              m_max;
		float              m_height;
		bool               m_histogram;

	};

}