#pragma once
#include "mvPlot.h"

namespace Marvel {

	class mvLineSeries : public mvSeries
	{

	public:

		mvLineSeries(const std::string& name, const std::vector<mvVec2>& points,
			mvColor color)
			: mvSeries(name, points), m_color(color)
		{
		}

		void draw() override
		{
			if (m_color.specified)
				ImPlot::SetNextLineStyle(m_color.toVec4());

			ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, m_weight);
			ImPlot::PlotLine(m_name.c_str(), m_xs.data(), m_ys.data(), m_xs.size());
			ImPlot::PopStyleVar();
		}

	private:

		mvColor m_color;

	};

}