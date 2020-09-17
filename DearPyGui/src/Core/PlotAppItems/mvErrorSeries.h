#pragma once
#include "mvPlot.h"

namespace Marvel {

	class mvErrorSeries : public mvSeries
	{

	public:

		mvErrorSeries(const std::string& name, const std::vector<mvVec4>& points, bool horizontal)
			: mvSeries(name, points), m_horizontal(horizontal)
		{
		}

		void draw() override
		{
			if(m_horizontal)
				ImPlot::PlotErrorBarsH(m_name.c_str(), m_xs.data(), m_ys.data(), m_extra1.data(), m_extra2.data(), m_xs.size());
			else
				ImPlot::PlotErrorBars(m_name.c_str(), m_xs.data(), m_ys.data(), m_extra1.data(), m_extra2.data(), m_xs.size());
		}

	private:

		bool m_horizontal;

	};

}