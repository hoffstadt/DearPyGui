#pragma once
#include "mvPlot.h"

namespace Marvel {

	class mvBarSeries : public mvSeries
	{

	public:

		mvBarSeries(const std::string& name, const std::vector<mvVec2>& points, bool horizontal)
			: mvSeries(name, points), m_horizontal(horizontal)
		{
		}

		mvSeriesType getSeriesType() override { return mvSeriesType::Bar; }

		void draw() override
		{
			if(m_horizontal)
				ImPlot::PlotBarsH(m_name.c_str(), m_xs.data(), m_ys.data(), m_xs.size(), m_weight);
			else
				ImPlot::PlotBars(m_name.c_str(), m_xs.data(), m_ys.data(), m_xs.size(), m_weight);
		}

	private:

		bool m_horizontal;

	};

}