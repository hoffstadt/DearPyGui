#pragma once
#include "mvPlot.h"

namespace Marvel {

	class mvHeatSeries : public mvSeries
	{

	public:

		mvHeatSeries(const std::string& name, const std::vector<float>* values, 
			int rows, int cols, double scale_min, double scale_max, const std::string& format,
			mvVec2 bounds_min, mvVec2 bounds_max, ImPlotYAxis_ axis)
			: 
			mvSeries(name, {values}, axis),
			m_rows(rows), 
			m_cols(cols), 
			m_scale_min(scale_min),
			m_scale_max(scale_max), 
			m_format(format), 
			m_bounds_min(bounds_min), 
			m_bounds_max(bounds_max)
		{
			m_minX = bounds_min.x;
			m_maxX = bounds_max.x;
			m_minY = bounds_min.y;
			m_maxY = bounds_max.y;
		}

		mvSeriesType getSeriesType() override { return mvSeriesType::Heat; }

		void draw() override
		{

			ImPlot::PlotHeatmap(m_name.c_str(), m_data[0].data(), m_rows, m_cols, m_scale_min, m_scale_max,
				m_format.c_str(), { m_bounds_min.x, m_bounds_min.y }, { m_bounds_max.x, m_bounds_max.y });
		
		}

	private:

		int         m_rows;
		int         m_cols;
		double      m_scale_min;
		double      m_scale_max;
		std::string m_format;
		mvVec2      m_bounds_min;
		mvVec2      m_bounds_max;

	};

}