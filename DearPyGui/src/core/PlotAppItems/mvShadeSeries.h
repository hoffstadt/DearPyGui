#pragma once
#include "mvPlot.h"

namespace Marvel {

	class mvShadeSeries : public mvSeries
	{

	public:

		mvShadeSeries(const std::string& name, mvColor color, mvColor fill, const std::vector<float>* x,
			const std::vector<float>* y1, const std::vector<float>* y2)
			: 
			mvSeries(name, {x, y1, y2}),
			m_color(color), 
			m_fill(fill)
		{
		}

		mvSeriesType getSeriesType() override { return mvSeriesType::Shade; }

		void draw() override
		{
			ImPlot::SetNextLineStyle(m_color.toVec4());
	
			ImPlot::SetNextFillStyle(m_fill.toVec4());
			ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, m_weight);
			ImPlot::PlotShaded(m_name.c_str(), m_data[0].data(), m_data[1].data(), 
				m_data[2].data(), (int)m_data[0].size());
			ImPlot::PopStyleVar();
		}

	private:

		mvColor m_color;
		mvColor m_fill;

	};

}