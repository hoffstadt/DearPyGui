#pragma once

#include "mvPlot.h"

namespace Marvel {

	class mvPieSeries : public mvSeries
	{

	public:

		mvPieSeries(const std::string& name, const std::vector<float>* values, double x,
			double y, double radius, bool normalize, double angle, const std::string& format,
			const std::vector<std::string>& labels, ImPlotYAxis_ axis)
			: 
			mvSeries(name, {values}, axis),
			m_x(x), 
			m_y(y),
			m_radius(radius), 
			m_normalize(normalize),
			m_angle(angle), 
			m_format(format), 
			m_labels(labels)
		{
			for (const auto& item : m_labels)
				m_clabels.push_back(item.c_str());
		}

		mvSeriesType getSeriesType() override { return mvSeriesType::Pie; }

		void draw() override
		{
			ImPlot::PlotPieChart(m_clabels.data(), m_data[0].data(), (int)m_labels.size(),
				m_x, m_y, m_radius, m_normalize, m_format.c_str(), m_angle);
		}

	private:

		double m_x;
		double m_y;
		double m_radius;
		bool m_normalize;
		double m_angle;
		std::string m_format;
		std::vector<std::string> m_labels;
		std::vector<const char*> m_clabels;

	};

}