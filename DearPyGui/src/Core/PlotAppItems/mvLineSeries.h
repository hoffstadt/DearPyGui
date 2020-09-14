#pragma once
#include "mvPlot.h"

namespace Marvel {

	class mvLineSeries : public mvSeries
	{

	public:

		mvLineSeries(const std::string& name, const std::vector<mvVec2>& points, float weight = 1.0f,
			mvColor color = MV_DEFAULT_COLOR, mvColor fill = MV_DEFAULT_COLOR)
			: mvSeries(name, points), m_lineWeight(weight), m_color(color), m_fill(fill)
		{
		}

		void draw() override
		{
			if (m_color.specified)
				ImPlot::SetNextLineStyle(m_color.toVec4());
			if (m_fill.specified)
				ImPlot::SetNextFillStyle(m_fill.toVec4());

			ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, m_lineWeight);

			if (m_fill.specified)
				ImPlot::PlotShaded(m_name.c_str(), m_xs.data(), m_ys.data(), m_xs.size());
			else
				ImPlot::PlotLine(m_name.c_str(), m_xs.data(), m_ys.data(), m_xs.size());


			ImPlot::PopStyleVar();
		}

	private:

		float   m_lineWeight;
		mvColor m_color;
		mvColor m_fill;

	};

}