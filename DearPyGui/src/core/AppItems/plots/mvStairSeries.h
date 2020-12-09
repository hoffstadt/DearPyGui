#pragma once

#include "mvPlot.h"

namespace Marvel {

	class mvStairSeries : public mvSeries
	{

	public:

		mvStairSeries(const std::string& name, const std::vector<float>* x,
			const std::vector<float>* y, mvColor color, ImPlotYAxis_ axis)
			: 
			mvSeries(name, {x, y}, axis),
			m_color(color)
		{
		}

		mvSeriesType getSeriesType() override { return mvSeriesType::Stair; }

		void draw() override
		{

			ImPlot::SetNextLineStyle(m_color.toVec4());
			ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, m_weight);
			ImPlot::PlotStairs(m_name.c_str(), m_data[0].data(), m_data[1].data(), (int)m_data[0].size());
			ImPlot::PopStyleVar();
		}

	private:

		mvColor m_color;

	};

}
