#pragma once
#include "mvPlot.h"

namespace Marvel {

	class mvStairSeries : public mvSeries
	{

	public:

		mvStairSeries(const std::string& name, const std::vector<mvVec2>& points,
			mvColor color)
			: mvSeries(name, points), m_color(color)
		{
		}

		mvStairSeries(const std::string& name, const std::vector<float>& points_x,
			const std::vector<float>& points_y, mvColor color)
			: mvSeries(name, points_x, points_y), m_color(color)
		{
		}

		mvSeriesType getSeriesType() override { return mvSeriesType::Line; }

		void draw() override
		{

			ImPlot::SetNextLineStyle(m_color.toVec4());
			ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, m_weight);
			ImPlot::PlotStairs(m_name.c_str(), m_xs.data(), m_ys.data(), (int)m_xs.size());
			ImPlot::PopStyleVar();
		}

	private:

		mvColor m_color;

	};

}
