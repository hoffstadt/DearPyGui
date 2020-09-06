#pragma once

#include <utility>

#include "mvAppItem.h"
#include "mvPythonTranslator.h"

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
		mvSimplePlot(const std::string& parent, const std::string& name, std::vector<float> value,
               std::string overlay, float scale_min, float scale_max, float height, bool histogram)
			: mvAppItem(parent, name), m_value(std::move(value)), m_overlay(std::move(overlay)), m_min(scale_min), m_max(scale_max), m_height(height), m_histogram(histogram)
		{
		}

		void draw() override
		{
			pushColorStyles();

			if(m_histogram)
				ImGui::PlotHistogram(m_label.c_str(), m_value.data(), m_value.size(), 0, m_overlay.c_str(), 
					m_min, m_max, ImVec2((float)m_width, (float)m_height));
			else
				ImGui::PlotLines(m_label.c_str(), m_value.data(), m_value.size(), 0, m_overlay.c_str(), 
					m_min, m_max, ImVec2((float)m_width, (float)m_height));

			popColorStyles();
		}

		void setPyValue(PyObject* value) override
		{
			m_value = ToFloatVect(value, m_name + " requires a list or tuple of floats.");
		}

		[[nodiscard]] PyObject* getPyValue() const override
		{
			if (!m_dataSource.empty())
			{
				if (!mvDataStorage::HasData(m_dataSource))
					mvDataStorage::AddData(m_dataSource, ToPyList(m_value));
				else
					UpdatePyFloatList(mvDataStorage::GetDataIncRef(m_dataSource), m_value);
				
				return mvDataStorage::GetData(m_dataSource);
			}

			return ToPyList(m_value);
		}

		void setValue(const std::vector<float>& value){ m_value = value;}
		[[nodiscard]] const std::vector<float>& getValue() const { return m_value; }

	private:

		std::vector<float> m_value;
		std::string        m_overlay;
		float              m_min;
		float              m_max;
		float              m_height;
		bool               m_histogram;

	};

}