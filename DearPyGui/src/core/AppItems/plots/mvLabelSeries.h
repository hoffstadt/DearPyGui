#pragma once

#include "mvPlot.h"

namespace Marvel {

	class mvLabelSeries : public mvSeries
	{

	public:

		mvLabelSeries(const std::string& name, const std::vector<float>* x, 
			const std::vector<float>* y, int xoffset = 0, int yoffset = 0, bool vertical = false, ImPlotYAxis_ axis = ImPlotYAxis_1)
			: 
			mvSeries(name, {x, y}, axis),
			m_xoffset(xoffset), 
			m_yoffset(yoffset),
			m_vertical(vertical)
		{
		}

		mvSeriesType getSeriesType() override { return mvSeriesType::Label; }

		void draw() override
		{
			ImPlot::PlotText(m_name.c_str(), m_data[0][0], m_data[0][1], m_vertical,
				ImVec2((float)m_xoffset, (float)m_yoffset));
		}

	private:

		int  m_xoffset;
		int  m_yoffset;
		bool m_vertical;

	};

}