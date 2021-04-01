#pragma once

#include "mvPlot.h"

namespace Marvel {

	class mvLineSeries : public mvSeries
	{

	public:

		mvLineSeries(const std::string& name, const std::vector<float>* x,
			const std::vector<float>* y, mvColor color, ImPlotYAxis_ axis)
			: 
			mvSeries(name, { x, y }, axis),
			m_color(color)
		{
		}

		mvSeriesType getSeriesType() override { return mvSeriesType::Line; }

		void draw(ImDrawList* drawlist, float x, float y) override
		{

			ImPlot::SetNextLineStyle(m_color);
			ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, m_weight);
			ImPlot::PlotLine(m_name.c_str(), m_data[0].data(), m_data[1].data(), (int)m_data[0].size());
			ImPlot::PopStyleVar();
		}

	private:

		mvColor m_color;

	};

}
