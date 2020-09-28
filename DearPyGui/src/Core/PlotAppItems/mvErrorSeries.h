#pragma once
#include "mvPlot.h"

namespace Marvel {

	class mvErrorSeries : public mvSeries
	{

	public:

		mvErrorSeries(const std::string& name, const std::vector<mvVec4>& points, bool horizontal, const mvColor& color)
			: mvSeries(name, points), m_horizontal(horizontal), m_color(color)
		{
		}

		mvSeriesType getSeriesType() override { return mvSeriesType::Error; }

		void draw() override
		{
			ImPlot::PushStyleColor(ImPlotCol_ErrorBar, m_color.toVec4());
			if(m_horizontal)
				ImPlot::PlotErrorBarsH(m_name.c_str(), m_xs.data(), m_ys.data(), m_extra1.data(), m_extra2.data(), m_xs.size());
			else
				ImPlot::PlotErrorBars(m_name.c_str(), m_xs.data(), m_ys.data(), m_extra1.data(), m_extra2.data(), m_xs.size());

			ImPlot::PopStyleColor();
		}

	private:

		bool    m_horizontal;
		mvColor m_color;

	};

}