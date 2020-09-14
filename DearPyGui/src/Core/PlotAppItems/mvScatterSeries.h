#pragma once
#include "mvPlot.h"

namespace Marvel {

	class mvScatterSeries : public mvSeries
	{

	public:

		mvScatterSeries(const std::string& name, const std::vector<mvVec2>& points, int marker = 2, float markerSize = 4.0f, float markerWeight = 1.0f,
			mvColor markerOutlineColor = MV_DEFAULT_COLOR, mvColor markerFillColor = MV_DEFAULT_COLOR)
			: mvSeries(name, points), m_marker(marker), m_markerSize(markerSize), m_markerWeight(markerWeight),
			m_markerOutlineColor(markerOutlineColor), m_markerFillColor(markerFillColor)
		{
		}

		void draw() override
		{
			if (m_markerOutlineColor.specified)
				ImPlot::PushStyleColor(ImPlotCol_MarkerOutline, m_markerOutlineColor);
			if (m_markerFillColor.specified)
				ImPlot::PushStyleColor(ImPlotCol_MarkerFill, m_markerFillColor);

			ImPlot::PushStyleVar(ImPlotStyleVar_Marker, m_marker);
			ImPlot::PushStyleVar(ImPlotStyleVar_MarkerSize, m_markerSize);
			ImPlot::PushStyleVar(ImPlotStyleVar_MarkerWeight, m_markerWeight);

			ImPlot::PlotScatter(m_name.c_str(), m_xs.data(), m_ys.data(), m_xs.size());

			if (m_markerOutlineColor.specified)
				ImPlot::PopStyleColor();
			if (m_markerFillColor.specified)
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