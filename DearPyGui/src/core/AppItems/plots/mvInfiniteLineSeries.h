#pragma once

#include "mvPlot.h"

namespace Marvel {

	class mvVLineSeries : public mvSeries
	{

	public:

		mvVLineSeries(const std::string& name, const std::vector<float>* x,
			mvColor color, ImPlotYAxis_ axis)
			:
			mvSeries(name, { x }, axis),
			m_color(color)
		{
		}

		mvSeriesType getSeriesType() override { return mvSeriesType::VLine; }

		void draw(ImDrawList* drawlist, float x, float y) override
		{

			ImPlot::SetNextLineStyle(m_color);
			ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, m_weight);
			ImPlot::PlotVLines(m_name.c_str(), m_data[0].data(), (int)m_data[0].size());
			ImPlot::PopStyleVar();
		}

	private:

		mvColor m_color;

	};

	class mvHLineSeries : public mvSeries
	{

	public:

		mvHLineSeries(const std::string& name, const std::vector<float>* x, 
			mvColor color, ImPlotYAxis_ axis)
			:
			mvSeries(name, { x }, axis),
			m_color(color)
		{
		}

		mvSeriesType getSeriesType() override { return mvSeriesType::HLine; }

		void draw(ImDrawList* drawlist, float x, float y) override
		{

			ImPlot::SetNextLineStyle(m_color);
			ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, m_weight);
			ImPlot::PlotHLines(m_name.c_str(), m_data[0].data(), (int)m_data[0].size());
			ImPlot::PopStyleVar();
		}

	private:

		mvColor m_color;

	};

}
