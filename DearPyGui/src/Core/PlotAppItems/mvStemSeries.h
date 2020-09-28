#pragma once
#include "mvPlot.h"

namespace Marvel {

	class mvStemSeries : public mvSeries
	{

	public:

		mvStemSeries(const std::string& name, const std::vector<mvVec2>& points, int marker,
			float markerSize, float markerWeight,
			mvColor markerOutlineColor, mvColor markerFillColor)
			: mvSeries(name, points), m_marker(marker), m_markerSize(markerSize), m_markerWeight(markerWeight),
			m_markerOutlineColor(markerOutlineColor), m_markerFillColor(markerFillColor)
		{
		}

		mvSeriesType getSeriesType() override { return mvSeriesType::Stem; }

		void draw() override
		{

			ImPlot::PushStyleColor(ImPlotCol_MarkerOutline, m_markerOutlineColor.toVec4());
			ImPlot::PushStyleColor(ImPlotCol_MarkerFill, m_markerFillColor.toVec4());

			ImPlot::PushStyleVar(ImPlotStyleVar_Marker, m_marker);
			ImPlot::PushStyleVar(ImPlotStyleVar_MarkerSize, m_markerSize);
			ImPlot::PushStyleVar(ImPlotStyleVar_MarkerWeight, m_markerWeight);

			ImPlot::PlotStems(m_name.c_str(), m_xs.data(), m_ys.data(), m_xs.size());

			ImPlot::PopStyleColor();
			ImPlot::PopStyleColor();
			ImPlot::PopStyleVar(3);
		}

	private:

		int   m_marker = 1;
		float m_markerSize = 4.0f;
		float m_markerWeight = 1.0f;
		mvColor m_markerOutlineColor = MV_DEFAULT_COLOR;
		mvColor m_markerFillColor = MV_DEFAULT_COLOR;

	};

}