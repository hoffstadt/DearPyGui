#pragma once
#include "mvPlot.h"

namespace Marvel {

	class mvShadeSeries : public mvSeries
	{

	public:

		mvShadeSeries(const std::string& name, const std::vector<mvVec2>& points,
			mvColor color, mvColor fill)
			: mvSeries(name, points), m_color(color), m_fill(fill)
		{
		}

		void draw() override
		{
			if (m_color.specified)
				ImPlot::SetNextLineStyle(m_color.toVec4());
	
			ImPlot::SetNextFillStyle(m_fill.toVec4());
			ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, m_weight);
			ImPlot::PlotShaded(m_name.c_str(), m_xs.data(), m_ys.data(), m_xs.size());
			ImPlot::PopStyleVar();
		}

	private:

		mvColor m_color;
		mvColor m_fill;

	};

}