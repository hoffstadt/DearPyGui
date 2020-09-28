#pragma once
#include "mvPlot.h"

namespace Marvel {

	class mvHeatSeries : public mvSeries
	{

	public:

		mvHeatSeries(const std::string& name, const std::vector<std::vector<float>>& points, 
			int rows, int cols, double scale_min, double scale_max, const std::string& format,
			mvVec2 bounds_min, mvVec2 bounds_max)
			: mvSeries(name, points), m_rows(rows), m_cols(cols), m_scale_min(scale_min),
			m_scale_max(scale_max), m_format(format), m_bounds_min(bounds_min), m_bounds_max(bounds_max)
		{
			m_datapoints = new float[m_rows*m_cols];
			int index = 0;
			for (size_t i = 0; i < points.size(); i++)
			{
				for (size_t j = 0; j < points[i].size(); j++)
				{
					m_datapoints[index] = points[i][j];
					index++;
				}
			}
		}

		mvSeriesType getSeriesType() override { return mvSeriesType::Heat; }

		void draw() override
		{

			ImPlot::PlotHeatmap(m_name.c_str(), m_datapoints, m_rows, m_cols, m_scale_min, m_scale_max,
				m_format.c_str(), { m_bounds_min.x, m_bounds_min.y }, { m_bounds_max.x, m_bounds_max.y });
		
		}

		~mvHeatSeries()
		{
			delete[] m_datapoints;
		}

	private:

		int         m_rows;
		int         m_cols;
		double      m_scale_min;
		double      m_scale_max;
		std::string m_format;
		mvVec2      m_bounds_min;
		mvVec2      m_bounds_max;
		float*      m_datapoints;

	};

}