#pragma once

#include "mvPlot.h"

namespace Marvel {

	class mvBarSeries : public mvSeries
	{

	public:

		mvBarSeries(const std::string& name, const std::vector<float>* x,
			const std::vector<float>* y, bool horizontal, ImPlotYAxis_ axis)
			: mvSeries(name, {x, y}, axis), m_horizontal(horizontal)
		{
		}

		mvSeriesType getSeriesType() override { return mvSeriesType::Bar; }

		void draw() override
		{
			if(m_horizontal)
				ImPlot::PlotBarsH(m_name.c_str(), m_data[0].data(), m_data[1].data(), (int)m_data[0].size(), m_weight);
			else
				ImPlot::PlotBars(m_name.c_str(), m_data[0].data(), m_data[1].data(), (int)m_data[0].size(), m_weight);
		}

	private:

		bool m_horizontal;

	};

}