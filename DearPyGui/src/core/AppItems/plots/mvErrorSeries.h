#pragma once
#include "mvPlot.h"

namespace Marvel {

	class mvErrorSeries : public mvSeries
	{

	public:

		mvErrorSeries(const std::string& name, const std::vector<float>* x, const std::vector<float>* y,
			const std::vector<float>* neg, const std::vector<float>* pos,
			bool horizontal, const mvColor& color, ImPlotYAxis_ axis)
			: 
			mvSeries(name, {x, y, neg, pos}, axis),
			m_horizontal(horizontal), 
			m_color(color)
		{
		}

		mvSeriesType getSeriesType() override { return mvSeriesType::Error; }

		void draw() override
		{
			ImPlot::PushStyleColor(ImPlotCol_ErrorBar, m_color.toVec4());
			if(m_horizontal)
				ImPlot::PlotErrorBarsH(m_name.c_str(), m_data[0].data(), m_data[1].data(), m_data[2].data(), 
					m_data[3].data(), (int)m_data[0].size());
			else
				ImPlot::PlotErrorBars(m_name.c_str(), m_data[0].data(), m_data[1].data(), m_data[2].data(),
					m_data[3].data(), (int)m_data[0].size());

			ImPlot::PopStyleColor();
		}

	private:

		bool    m_horizontal;
		mvColor m_color;

	};

}