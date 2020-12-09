#pragma once

#include "mvPlot.h"

namespace Marvel {

	class mvScatterSeries : public mvSeries
	{

	public:

		mvScatterSeries(const std::string& name, const std::vector<float>* x, 
			const std::vector<float>* y, int marker, float markerSize, float markerWeight,
			mvColor markerOutlineColor, mvColor markerFillColor, ImPlotYAxis_ axis)
			: 
			mvSeries(name, {x, y}, axis),
			m_marker(marker), 
			m_markerSize(markerSize), 
			m_markerWeight(markerWeight),
			m_markerOutlineColor(markerOutlineColor),
			m_markerFillColor(markerFillColor)
		{
		}

		mvSeriesType getSeriesType() override { return mvSeriesType::Scatter; }

		void draw() override
		{
			ImPlot::PushStyleColor(ImPlotCol_MarkerOutline, m_markerOutlineColor.toVec4());
			ImPlot::PushStyleColor(ImPlotCol_MarkerFill, m_markerFillColor.toVec4());

			ImPlot::PushStyleVar(ImPlotStyleVar_Marker, m_marker);
			ImPlot::PushStyleVar(ImPlotStyleVar_MarkerSize, m_markerSize);
			ImPlot::PushStyleVar(ImPlotStyleVar_MarkerWeight, m_markerWeight);

			ImPlot::PlotScatter(m_name.c_str(), m_data[0].data(), m_data[1].data(), (int)m_data[0].size());

			ImPlot::PopStyleColor();
			ImPlot::PopStyleColor();
			ImPlot::PopStyleVar(3);
		}

	private:

		int   m_marker = 2;
		float m_markerSize = 4.0f;
		float m_markerWeight = 1.0f;
		mvColor m_markerOutlineColor = MV_DEFAULT_COLOR;
		mvColor m_markerFillColor = MV_DEFAULT_COLOR;

	};

}
